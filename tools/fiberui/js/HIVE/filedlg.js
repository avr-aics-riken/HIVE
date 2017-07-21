/*jslint devel:true*/

window.FileDialog = (function () {
	"use strict";
	var ItemClass = "file_dialog_item",
		BackClass = "file_dialog_back",
		LabelClass = "file_dialog_label",
		FileIconClass = "file_dialog_file",
		DirIconClass = "file_dialog_dir",
		FileNameInputClass = "file_name_input",
		FileListClass = "file_dialog_file_list",
		FileDialog = function (name, ignoreDotFile, rootnode) {
			this.name = name;
			this.tarPath = "";
			this.exttype = "";
			this.dir_only = false;
			this.ignoreDotFile = ignoreDotFile;
			this.domNode = rootnode;
		};

	FileDialog.prototype.set_filter_input = function (input_elem) {
		function keyupFunc(self) {
			return function () {
				var keyw = input_elem.value,
					ls = (self.domNode.getElementsByClassName(FileListClass))[0],
					fileitems = ls.getElementsByClassName(ItemClass),
					flabels = ls.getElementsByClassName(LabelClass),
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
		if (input_elem) {
			input_elem.addEventListener('keyup', keyupFunc(this));
		}
	}

	//--------------
	FileDialog.prototype.update_dir_list = function (jsonlist, nextFileListCallback) {
		var unode, list, i, newbtn, ls, plist;
		this.FileList = nextFileListCallback;
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

			newbtn.setAttribute('class', ItemClass);
			newbtn.setAttribute('draggable', "false");

			fileicon.setAttribute('class', BackClass);
			newbtn.appendChild(fileicon);

			filelabel.setAttribute('class', LabelClass);
			filelabel.innerHTML = "..";
			newbtn.appendChild(filelabel);

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

			newbtn.setAttribute('class', ItemClass);
			newbtn.setAttribute('draggable', "false");
			newbtn.appendChild(fileicon);

			filelabel.setAttribute('class', LabelClass);
			filelabel.innerHTML = name;
			newbtn.appendChild(filelabel);

			if (type === "file") {
				fileicon.setAttribute('class', FileIconClass);
				newbtn.addEventListener('click', (function (self, p) {
					return function () {
						var names = p.split('/');
						(self.domNode.getElementsByClassName(FileNameInputClass))[0].value = names[names.length - 1];
					};
				}(self, path)));
			} else if (type === "dir") {
				fileicon.setAttribute('class', DirIconClass);
				newbtn.addEventListener('click', (function (self, p) {
					return function () {
						console.log('[DEBUG] filedlg.js:', p);
						self.FileList(p);
					};
				}(self, path)));
			}
			return newbtn;
		}

 		// clear
		ls = (this.domNode.getElementsByClassName(FileListClass))[0];
		ls.innerHTML = '';

		// up dir
		plist = jsonlist;
		this.tarPath = plist.path;
		unode = makeUpNode(this);
		list = plist.list;
		(this.domNode.getElementsByClassName(FileNameInputClass))[0].value = plist.path;

		ls.appendChild(unode);
		for (i in list) {
			if (list.hasOwnProperty(i)) {
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
