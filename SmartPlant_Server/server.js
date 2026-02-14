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

let history = [];

if (fs.existsSync(HISTORY_FILE)) {
    try {
        history = JSON.parse(fs.readFileSync(HISTORY_FILE));
    } catch (e) { console.error("History file corrupted, starting new."); }
}

function saveToHistory(moisture) {

    const timestamp = new Date().toLocaleString('fa-IR');
    
    const entry = {
        time: timestamp,
        moisture: moisture,
        status: moisture < 30 ? "آبیاری شد 💧" : "ثبت وضعیت ✅"
    };

    history.unshift(entry);
    if (history.length > 50) history.pop();
    
    fs.writeFileSync(HISTORY_FILE, JSON.stringify(history));
    
    io.emit('history-update', history);
}

console.log("Connecting to MQTT...");
const mqttClient = mqtt.connect(BROKER_URL);

mqttClient.on('connect', () => {
    console.log(`MQTT Connected! Listening on: ${TOPIC_DATA}`);
    mqttClient.subscribe(TOPIC_DATA);
});

mqttClient.on('message', (topic, message) => {
    const msgString = message.toString();
    console.log(`Data received: ${msgString}`);
    
    try {
        const data = JSON.parse(msgString);
        
        io.emit('sensor-data', data);
        
        saveToHistory(data.moisture);
        
    } catch (e) {
        console.error("Invalid JSON from ESP32");
    }
});

io.on('connection', (socket) => {
    console.log('Mobile App Connected');
    
    socket.emit('history-update', history);

    socket.on('send-settings', (settings) => {
        console.log('Settings update received:', settings);
        
        const commandJson = JSON.stringify(settings);
        mqttClient.publish(TOPIC_COMMAND, commandJson);
    });
});

server.listen(PORT, () => {
    console.log(`-----------------------------------------------`);
    console.log(`Server running. Mobile App URL: http://localhost:${PORT}`);
    console.log(`-----------------------------------------------`);
});