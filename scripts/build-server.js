// This script should be invoked at HIVE top directory.
// 
// $ node ./scripts/build-server.js
//
require('shelljs/global')

var app = require('http').createServer(handler)

// -- config -----
var port = 4001;
// ---------------


app.listen(port);
console.log("port:" + port)

function worker(res) {

  var result = ""

  res.write('--> git pull');
  res.write(exec('git pull origin master').output)

  res.write(exec('rm -rf build').output)
  res.write(exec('./scripts/cmake_linux-x64.sh').output)

  var child = exec('cd build; make', {async:true});
  child.stdout.on('data', function(data) {
    res.write(data)
  });
  child.on('exit', function() {
    res.end()
  });

}

function handler (req, res) {

  if (req.url == '/v2/keys/build_notify') {

    res.writeHead(200, {'Content-Type': 'text/plain'});

    worker(res);

  } else {
    res.writeHead(404, {'Content-Type': 'text/plain'});
    res.end();
  }

}

