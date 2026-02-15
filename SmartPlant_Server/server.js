const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const mqtt = require('mqtt');
const fs = require('fs');
const path = require('path');

const PORT = 3000;
const BROKER_URL = 'mqtt://test.mosquitto.org';
const TOPIC_DATA = 'uni/project/smartplant/data';
const TOPIC_COMMAND = 'uni/project/smartplant/command';
const HISTORY_FILE = path.join(__dirname, 'history.json');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

app.use(express.static('public'));
app.use(express.json());

let eventHistory = []; 
let chartData = [];    
let lastWateringLogTime = 0; 

if (fs.existsSync(HISTORY_FILE)) {
    try {
        const savedData = JSON.parse(fs.readFileSync(HISTORY_FILE));
        eventHistory = savedData.events || [];
    } catch (e) {}
}

console.log("Starting Server...");
const mqttClient = mqtt.connect(BROKER_URL);

mqttClient.on('connect', () => {
    console.log(`MQTT Connected! Listening...`);
    mqttClient.subscribe(TOPIC_DATA);
});

mqttClient.on('message', (topic, message) => {
    try {
        const data = JSON.parse(message.toString());
        const now = Date.now();
        const timeStr = new Date().toLocaleTimeString('fa-IR');

        const newPoint = {
            ts: now,
            label: timeStr,
            value: data.moisture
        };

        chartData.push(newPoint);
        const fiveMinutesAgo = now - (5 * 60 * 1000);
        chartData = chartData.filter(point => point.ts > fiveMinutesAgo);

        io.emit('sensor-data', { 
            current: data, 
            point: newPoint 
        });

        if (data.empty === true) {
             if (now - lastWateringLogTime > 300000) { 
                lastWateringLogTime = now;
                const entry = {
                    time: new Date().toLocaleString('fa-IR'),
                    status: "⚠️ خطا: مخزن آب خالی است!",
                    moisture: data.moisture + "%"
                };
                eventHistory.unshift(entry);
                if(eventHistory.length > 20) eventHistory.pop();
                io.emit('history-update', eventHistory);
             }
        }
        else if (data.moisture < 30) {
            if (now - lastWateringLogTime > 60000) { 
                lastWateringLogTime = now;
                const entry = {
                    time: new Date().toLocaleString('fa-IR'),
                    moisture: data.moisture,
                    status: "آغاز آبیاری خودکار 💧"
                };
                eventHistory.unshift(entry);
                if (eventHistory.length > 20) eventHistory.pop();
                fs.writeFileSync(HISTORY_FILE, JSON.stringify({ events: eventHistory }));
                io.emit('history-update', eventHistory);
            }
        }

    } catch (e) { console.error(e); }
});

io.on('connection', (socket) => {
    socket.emit('history-update', eventHistory);
    socket.emit('init-chart', chartData);

    socket.on('send-settings', (settings) => {
        console.log("Settings Update:", settings);
        mqttClient.publish(TOPIC_COMMAND, JSON.stringify(settings));
    });
});

server.listen(PORT, () => console.log(`Server Running: http://localhost:${PORT}`));