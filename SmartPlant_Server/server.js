const mqtt = require('mqtt');

const brokerUrl = 'mqtt://broker.emqx.io';
const topic = 'uni/project/smartplant/data'; 

console.log("Starting Server...");
const client = mqtt.connect(brokerUrl);

client.on('connect', () => {
    console.log(`Connected to Broker! Listening on: ${topic}`);
    client.subscribe(topic);
});

client.on('message', (topic, message) => {
    console.log(`[${new Date().toLocaleTimeString()}] New Data: ${message.toString()}`);
});