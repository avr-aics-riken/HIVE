/*jslint devel:true*/

var FileDialog = (function () {
	"use strict";
	var tarPath = "",
		FileDialog = function (name, ignoreDotFile) {
			this.name = name;
			this.exttype = "";
			this.dir_only = false;
			this.ignoreDotFile = ignoreDotFile;
			this.domNode = null;
			this.cssNode = null;
		},
		xrequet = function (url, callback_online, callback_offline) {
			if (callback_offline === undefined) {
				callback_offline = callback_online;
			}
			var xhr = new XMLHttpRequest();
			xhr.open("GET", url, true);
			xhr.onreadystatechange = function () {
				if (xhr.readyState === 4 && xhr.status === 200 && callback_online) { // online
					callback_online(this.responseText);
				}
				if (xhr.readyState === 4 && xhr.status === 0 && callback_offline) { // offline
					callback_offline(this.responseText);
				}
			};
			xhr.send(null);
		};

	FileDialog.prototype.OpenDir = function (dirListCallback, callback) {
		this.Show("OpenDir", "", dirListCallback, callback);
	};
	FileDialog.prototype.OpenFile = function (exttype, dirListCallback, callback) {
		this.Show("Open", exttype, dirListCallback, callback);
	};
	FileDialog.prototype.SaveFile = function (exttype, dirListCallback, callback) {
		this.Show("Save", exttype, dirListCallback, callback);
	};

	FileDialog.prototype.Show = function (mode, exttype, dirListCallback, callback) {
		var htmlpath = "filedlg.html",
			csspath  = "css/filedlg.css",
			dlgtemplate,
			ff,
			fileref;

		function appendDlg(self, mode, exttype, fileListCallback, callback) {
			var d = document.createElement('div'),
				fdtemplate = document.getElementById('filedlg-template'),
				savebtn,
				cancelbtn,
				btnname,
				filetext,
				fileext,
				filtertext;
			
			if (fdtemplate === null) {
				console.log('NOT Found filedlg-template');
				return;
			}
			d.innerHTML = fdtemplate.innerHTML;
			self.domNode = d;
			savebtn   = (d.getElementsByClassName("filedlg_button_save"))[0];
			cancelbtn = (d.getElementsByClassName("filedlg_button_cancel"))[0];
			btnname = (savebtn.getElementsByClassName("filedlg_text_button_action"))[0];
			filetext = (self.domNode.getElementsByClassName('filedlg_input_filedialog'))[0];
			fileext = (self.domNode.getElementsByClassName('filedlg_filepath_ext'))[0];
			filtertext = d.getElementsByClassName('filedlg_filter')[0];
			fileext.innerHTML = exttype;
			self.exttype = exttype;

			console.log(filtertext);
			
			function keyupFunc(self) {
				return function () {
					var keyw = filtertext.value,
						ls = (self.domNode.getElementsByClassName('filedlg_filebrowser filedlg_filedialog'))[0],
						fileitems = ls.getElementsByClassName('filedlg_fileitem'),
						flabels = ls.getElementsByClassName('filedlg_filelabel'),
						fname,
						i;
					// visible reset
					for (i = 0; i < fileitems.length; i += 1) {
						fileitems[i].style.display = '';
					}
					if (keyw === '') {
						return;
					}
					for (i = 0; i < flabels.length; i += 1) {
						fname = flabels[i].innerHTML;
						keyw = keyw.toLowerCase();
						fname = fname.toLowerCase();
						if (fname.indexOf(keyw) === -1 && fname !== '..') {
							flabels[i].parentNode.style.display = 'none';
						}
					}
				};
			}
			function saveClickFunc(self) {
				return function () {
					//console.log('save!!!!');
					var fpath = (self.domNode.getElementsByClassName('filedlg_filepath_filedialog'))[0].innerHTML,
						fname = (self.domNode.getElementsByClassName('filedlg_input_filedialog'))[0].value,
						fileTypes,
						len;

					if (fname === '') {
						return;
					}
					// append ext
					fileTypes = fname.split(".");
					len = fileTypes.length;
					console.log(fileTypes, len, self.exttype);
					if (self.exttype !== "" && ("." + fileTypes[fileTypes.length - 1]) !== self.exttype) {
						fname += self.exttype;
					}

					if (fpath.slice(-1) !== '/') {
						fpath += '/';
					}
					fpath += fname;

					console.log('Select->' + fpath);
					if (callback) {
						callback(fpath);
						document.body.removeChild(self.domNode); // close
					}
				};
			}
			function cancelClickFunc(self) {
				return function () {
					//console.log('cancel!!!!');
					//console.log(self);
					document.body.removeChild(self.domNode); // close
				};
			}
			
			if (filtertext) {
				filtertext.addEventListener('keyup', keyupFunc(self));
			}
			if (savebtn) {
				if (mode.toLowerCase() === "open") {
					self.dir_only = false;
					btnname.innerHTML = "Open";
					filetext.setAttribute("readonly", "readonly");
					//fileext.style.display = "none";
				} else if (mode.toLowerCase() === "save") {
					self.dir_only = false;
					btnname.innerHTML = "Save";
				} else if (mode.toLowerCase() === "opendir") {
					self.dir_only = true;
					btnname.innerHTML = "Open";
					filetext.value = "";
					filetext.style.display = "none";
					fileext.style.display = "none";
				} else {
					console.log("Error: Unknown file dialog type.");
				}

				savebtn.addEventListener('click', saveClickFunc(self));
			}
			if (cancelbtn) {
				cancelbtn.addEventListener('click', cancelClickFunc(self));
			}
			document.body.appendChild(d);

			// get file list
			
			if (!dirListCallback) {
				console.log('Error: dirListCallback is null.');
			} else {
				self.FileList = dirListCallback;
				self.FileList('/');
			}
		}

		function loadResult(self, initCallback, callback) {
			return function (htmldata) {
				dlgtemplate = document.createElement('div');
				dlgtemplate.setAttribute('id', 'filedlg-template');
				dlgtemplate.innerHTML = htmldata;
				ff.appendChild(dlgtemplate);
				appendDlg(self, mode, exttype, initCallback, callback);
			};
		}
		dlgtemplate = document.getElementById('filedlg-template');
		if (dlgtemplate !== null) { // Cached
			appendDlg(this, mode, exttype, dirListCallback, callback);
		} else { // not cached
			ff = document.getElementById('filedlg-folder');
			if (ff === null) {
				ff = document.createElement('div');
				ff.setAttribute('id', 'filedlg-folder');
				ff.style.display = "none";
				document.body.appendChild(ff);
			}

			// Add CSS
			fileref = document.createElement('link');
			fileref.setAttribute('rel', 'stylesheet');
			fileref.setAttribute('type', 'text/css');
			fileref.setAttribute('href', csspath);
			this.cssNode = fileref;
			ff.appendChild(fileref);

			xrequet(htmlpath, loadResult(this, dirListCallback, callback));
		}

	};

	/*FileDialog.prototype.registerSocketEvent = function (websocket) {
		this.ws = websocket;
		var ev, eventname = this.name + ':updatedFileList';
		//console.log('FileDialog:' + eventname);

		ev = function (thisptr) {
			return function (data) {
				thisptr.updateDirlist(data);
			};
		};
		socket.on(eventname, ev(this));
	};*/
	FileDialog.prototype.dispPath = function (p) {
		(this.domNode.getElementsByClassName('filedlg_filepath_filedialog'))[0].innerHTML = p;
	};
	/*
	FileDialog.prototype.FileList = function (path) {
		//this.dispPath(path);
		//console.log("FileList:"+path);
		//this.socket.emit(this.name + ":requestFileList", path);
	};*/

	//--------------
	FileDialog.prototype.updateDirlist = function (jsonlist) {
		var unode, list, i, newbtn, ls, filtertext, plist;
		function getUpDir(path) { // fix me beautiful
			var p = path.split("/"),
				uppath = "/",
				i;
			if (path === "/") {
				return "/";
			}
			if (p[p.length - 1] === "") {
				p.pop();
			}

			for (i = 0; i < p.length - 1; i += 1) {
				if (p[i] !== "") {
					uppath += p[i] + '/';
				}
			}
			if (uppath === "//") {
				uppath = "/";
			}
			return uppath;
		}
		function makeUpNode(self) {
			var newbtn = document.createElement('div'),
				fileicon = document.createElement('div'),
				filelabel = document.createElement('p'),
				upath = getUpDir(self.tarPath);

			newbtn.setAttribute('class', "filedlg_fileitem");
			newbtn.setAttribute('draggable', "false");

			fileicon.setAttribute('class', 'filedlg_back');
			newbtn.appendChild(fileicon);

			filelabel.setAttribute('class', "filedlg_filelabel");
			filelabel.innerHTML = "..";
			newbtn.appendChild(filelabel);

			//console.log("UPATH="+upath);
			//newbtn.setAttribute('onclick', 'openfileDialog("' + upath + '")');
			newbtn.addEventListener('click', (function (self, p) {
				return function () {
					self.FileList(p);
				};
			}(self, upath)));
			return newbtn;
		}
		function makeNode(self, name, path, type) {
			var newbtn = document.createElement('div'),
				fileicon = document.createElement('div'),
				filelabel = document.createElement('p');

			newbtn.setAttribute('class', "filedlg_fileitem");
			newbtn.setAttribute('draggable', "false");

			newbtn.appendChild(fileicon);

			filelabel.setAttribute('class', "filedlg_filelabel");
			filelabel.innerHTML = name;
			newbtn.appendChild(filelabel);

			if (type === "file") {
				fileicon.setAttribute('class', 'filedlg_file');
				newbtn.addEventListener('click', (function (self, p) {
					return function () {
						var names = p.split('/');
						(self.domNode.getElementsByClassName('filedlg_input_filedialog'))[0].value = names[names.length - 1];
					};
				}(self, path)));
			} else if (type === "dir") {
				fileicon.setAttribute('class', 'filedlg_dir');
				newbtn.addEventListener('click', (function (self, p) {
					return function () {
						console.log('[DEBUG] filedlg.js:', p);
						self.FileList(p);
					};
				}(self, path)));
			}
			return newbtn;
		}

		filtertext = this.domNode.getElementsByClassName('filedlg_filter')[0];
		filtertext.value  = '';

		ls = (this.domNode.getElementsByClassName('filedlg_filebrowser filedlg_filedialog'))[0];
		ls.innerHTML = ''; // clear

		// up dir
		plist = jsonlist;//JSON.parse(jsonlist);
		this.tarPath = plist.path;
		unode = makeUpNode(this);
		list = plist.list;
		this.dispPath(plist.path);

		ls.appendChild(unode);
		for (i in list) {
			if (list.hasOwnProperty(i)) {
				//console.log('ADDDDD:'+list[i]);
				if (list[i].type !== "file" && list[i].type !== "dir") {
					console.log("Unknown file type -> " + list[i].type);
				} else {
					if (list[i].name.charAt(0) === "." && this.ignoreDotFile) {
						continue;
					}
					if (list[i].type === "file" && this.dir_only) { // ignore files
						continue;
					}
					if (this.exttype !== "" && list[i].type === "file" && this.exttype.toLowerCase() !== list[i].exttype.toLowerCase()) {
						continue;
					}
					newbtn = makeNode(this, list[i].name, list[i].path, list[i].type);

					ls.appendChild(newbtn);
				}
			}
		}
	};
	return FileDialog;
	
}());