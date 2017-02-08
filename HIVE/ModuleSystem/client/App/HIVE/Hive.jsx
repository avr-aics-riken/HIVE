import EventEmitter from 'eventemitter3'
import HiveConnect from "./HiveConnect.js"

/**
 * デフォルトコールバック
 */
function defaultCallback(src) {
	return function (err, res) {
		if (err) {
			console.error('runscript error:', err, {script: src});
			console.trace();
		} else {
			//console.log('runscript result:', res, {script: src});
		}
	};
}

/**
 * scriptを実行する
 */
function runScriptInternal(conn, src, callback) {
	conn.rendererMethod('runscript', {script: src}, callback || defaultCallback(src));
}

/**
 * shaderlistを取得
 */
function getShaderList(conn, callback) {
	conn.masterMethod('requestShaderList', {}, callback || defaultCallback(src));
}

/**
 * filelistを取得
 */
function getFileListInternal(conn, dir, callback) {
	conn.masterMethod('requestFileList', { path : dir }, callback || defaultCallback(src));
}

export default class Hive extends EventEmitter {
	constructor() {
		super();
		this.conn = null;
        this.shaderlist = [];
	}

	connect(wsurl, ipcAddress, ogl) {
		this.conn = new window.HiveConnect({url:wsurl, ipc:'', opengl:ogl});

		this.conn.method('registerRender', (res) => {

            this.shaderlist = [];
            getShaderList(this.conn, (err, shaderlist) => {
                var i;
                for (i in shaderlist) {
                    //console.log(shaderlist[i].path);
                    this.shaderlist.push(shaderlist[i].path); // relative path
                }
            });

			console.log('registerRender!!!!!', res);
			// HiveConnectにレンダラが関連付けられた
			var rendererId = res.id;
			console.log('rendererId = ', rendererId);
			this.conn.setRendererId(parseInt(res.id)); // これは必要！TODO
		});

		this.conn.method('updateInfo', (function (core, infoCallback) {
			console.log('updateInfo');
		}));

        this.conn.method('analyzedInfo', (data) => {
			//console.log('analyzedInfo', data);
            this.emit(Hive.ANALYZED_DATA_RECIEVED, data);
		});

        this.conn.method('rendererLog', (data) => {
		    //console.log('processlog>', data);
            this.emit(Hive.RENDERER_LOG_RECIEVED, data);
		});

		this.conn.method('renderedImage', (param, data) => {
			// 画像を受け取る ws version
			//console.log("renderImage recieved", param, data);
			this.emit(Hive.IMAGE_RECIEVED, null, param, data);
			/*
				var w, h, cmd;
				if (param.type === 'jpg') {
					resultElement.src = URL.createObjectURL(new Blob([data], {type: "image/jpeg"}));
				}

				// Refine render
				if (!param.canceled) {
					if (param.width <= hiveCore.screenSize[0] && param.height <= hiveCore.screenSize[1]) {
						w = Math.min(param.width * 2, hiveCore.screenSize[0] * 2);
						h = Math.min(param.height * 2, hiveCore.screenSize[1] * 2);
						cmd = HiveCommand.renderCamera(w, h, hiveCore.activeCamera, this.useIpc, true);
						runScript(hiveCore.conn, cmd);
					}
				}
			*/
		});
	}

    runScript(script, callback) {
        runScriptInternal(this.conn, script, (err, data) => {
            if (err) {
                console.error("runScript error:", err, '\nscript:', script);
                if (callback) {
                    callback(err, null);
                }
            } else {
                //console.log("runScript result:", data);
                if (callback) {
                    callback(null, data);
                }
            }
        });
    }

    getShaderList() {
        return this.shaderlist;
    }

	getFileList(dir, callback) {
		getFileListInternal(this.conn, dir, callback);
	}

	/**
	 * HIVEを再起動
	 */
	rebootHIVE(callback) {
		let param = {mode: 'client', opengl: this.conn.openGLMode, ipc:this.conn.ipcAddress};
		this.conn.masterMethod('rebootHIVE', param, callback || defaultCallback(src));
	}
	
	/*testRender() {
		runScriptInternal(this.conn,
			`
			package.path = './?.lua;' .. package.path
			HIVE_ObjectTable = {} -- Global: All Object List
			HIVE_ObjectTimeline = {}
			HIVE_DataTable   = {} -- Global: Data List
			HIVE_ImageSaver = ImageSaver()
			function HIVE_fetchEvent(progress)
				return true
			end
			hcmd = require('HiveCommand');
			(function ()
			hcmd.NewScene()
			hcmd.CreateCamera("view")
			hcmd.CameraLookat("view",-72.0,23.0,171.0,0,0,0,0,1,0,60)
			hcmd.CameraClearColor("view",0,0,0,1)
			hcmd.CameraScreenSize("view",512,512)
			hcmd.CameraOutputFilename("view","view.jpg")
			local obj = "/home/khatta/work/source/HIVE/hrender/test/bunny.obj"
			local frag = "/home/khatta/work/source/HIVE/hrender/test/normal.frag"
			hcmd.LoadOBJ("bunny_obj0",obj,frag)
			hcmd.SetModelTranslation("bunny_obj0",0,0,0,0,0,0,1,1,1)
			hcmd.SetModelShader("bunny_obj0",frag)
			clearCache()

			hcmd.SetModelUniformFloat("bunny_obj0","power",1)
			hcmd.RenderCamera(512, 512, "view", "jpg", false)
			end)();
			`, function (err, data) {
				console.log("runScript end");
		});
	}

	changeNode() {
		console.log("change node at HiveCore");
	}*/
}
Hive.NODE_CHANGED = "core_node_changed";
Hive.IMAGE_RECIEVED = "core_image_revieved";
Hive.RENDERER_LOG_RECIEVED = "renderer_log_recieved";
Hive.ANALYZED_DATA_RECIEVED = "analyzed_data_recieved";
