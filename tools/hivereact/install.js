var exec = require('child_process').exec,
    os = require('os'),
    opt = '';

if (os.platform() === 'win32') {
    opt = ' --msvs_version=2013';
}

var cmds =[ 'cd node_modules/nanomsg && node-gyp rebuild --target=0.36.4 --arch=x64 --dist-url=https://atom.io/download/atom-shell rebuild ' + opt,
	    'cd node_modules/buffercopy && node-gyp rebuild --target=0.36.4 --arch=x64 --dist-url=https://atom.io/download/atom-shell rebuild' + opt],
    cmd;

if (os.platform() !== 'linux') {
	for (i in cmds) {
	    cmd = cmds[i];
	    exec(cmd, function (err, stdout, stderr) {
		if(stdout){
		    console.log('stdout: ' + stdout);
		}
		if(stderr){
		    console.log('stderr: ' + stderr);
		}
		if (err !== null) {
		    console.log('Exec error: ' + err);
		}
	    });
	}
}
