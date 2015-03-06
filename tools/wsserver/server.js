var WebSocketServer = require('ws').Server
  , wss = new WebSocketServer({port: 8080});
wss.on('connection', function(ws) {
    ws.send('something');
    ws.on('message', function(message) {
        console.log('received: %s', message);
    });
});
