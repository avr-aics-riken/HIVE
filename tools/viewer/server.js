//
// Simple image viewer using socket.io.
//
// Works on node.js v0.10.35+
//

var app = require('http').createServer(handler)
  , io = require('socket.io').listen(app, {'log level': 2})
  , fs = require('fs')
  
// Config
var port        = 8082;

function generateDataURI(mime, data)
{
  var datauri = 'data:' + mime + ';base64,' + data;
  return datauri;
}

io.sockets.on('connection', function (socket) {

  console.log('server: sock io connect');

});

app.listen(port);

console.log("port:" + port);

function handler (req, res) {
  console.log(req)
  var filename = req.url
  var tmp = req.url.split('.');
  var type = tmp[tmp.length-1];
  //console.log(filename)
  //console.log(type)
  if (req.url == '/postparam') { // HIVE hack

    var chunks = []
    req.on('data', function(chunk) {
      chunks.push(chunk);
    });

    req.on('end', function() {
      //console.log(chunks);
      var data = Buffer.concat(chunks);

      // Assume data is base64 encoded jpeg image.
      var uri = generateDataURI('image/jpeg', data.toString())
      io.emit('render_image', uri);
      
    });

    // get image
    res.writeHead(200, {'Content-Type': 'text/html'});
    res.end();
    return;
  } 

  if (req.url == '/') {
    filename = '/index.html'
  }
  fs.readFile(__dirname + filename,
  function (err, data) {
    if (err) {
      res.writeHead(500);
      return res.end('Error loading ' + filename);
    }
    switch (type) {
      case 'html':
        res.writeHead(200, {'Content-Type': 'text/html'});
        break;
      case 'js':
        //console.log('js:' + req.url)
        res.writeHead(200, {'Content-Type': 'text/javascript'});
        break;
      case 'css':
        res.writeHead(200, {'Content-Type': 'text/css'});
        break;
    }
    res.end(data);
  });
}

