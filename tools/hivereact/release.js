var packager = require('electron-packager');
var config = require('./package.json');
var ncp = require('ncp').ncp;

packager({
	dir: '.',
	out: './dist',
	name: "HiveReact",
	platform: 'win32',
	arch: 'x64',
	version: '0.36.5',
	"app-version" : config.version,
	overwrite: true,
	asar: true,
	prune: true,
	ignore: "node_modules/(electron-packager|electron-prebuilt|\.bin)|release.js",
	"version-string" : {
		CompanyName: "RIKEN",
		FileDescription: "HiveReact",
		OriginalFilename: "HiveReact",
		FileVersion: config.version,
		ProductVersion: config.version,
		ProductName: "HiveReact",
		InternalName: "HiveReact",
		LegalCopyright : "RIKEN All rights reserved."
	}
}, function done (err, appPath) {
    if (err) {
        console.error(err);
    } else {
        var filter = function(name) {
          return name.indexOf("LICENSE") < 0;
        };
		ncp("./dist/HIVEReact-win32-x64", "../../bin/", { filter : filter, clobber : true } , function (err) {
			if (err) {
				console.error(err);
			}
			console.log("done!");
		});
	}
});
