import EventEmitter from 'eventemitter3'
import NodeSerializer from "./NodeSerializer.jsx"
import HiveConnect from "../HIVE/HiveConnect.js"

/**
 * デフォルトコールバック
 */
function defaultCallback(src) {
	return function (err, res) {
		if (err) {
			console.error('runscript error:', err, {script: src});
			console.trace();
		} else {
			console.log('runscript result:', res, {script: src});
		}
	};
}

/**
 * scriptを実行する
 */
function runScriptInternal(conn, src, callback) {
	conn.rendererMethod('runscript', {script: src}, callback || defaultCallback(src));
}

export default class HiveCore extends EventEmitter {
	constructor() {
		super();
		this.conn = null;
	}

	connect(wsurl, ipcAddress, ogl) {
		this.conn = new window.HiveConnect({url:wsurl, ipc:ipcAddress, opengl:ogl});

		this.conn.method('registerRender', (res) => {
			console.log('registerRender!!!!!', res);
			// HiveConnectにレンダラが関連付けられた
			var rendererId = res.id;
			console.log('rendererId = ', rendererId);
			this.conn.setRendererId(parseInt(res.id)); // これは必要！TODO

			runScriptInternal(this.conn, "print('!!!!hogehoge!!!!'); return 'hoge'", function (err, ret) {
				console.log('RET=', ret);
			});

			this.testRender();
		});

		this.conn.method('updateInfo', (function (core, infoCallback) {
			console.log('updateInfo');
		}));

		this.conn.method('renderedImage', function (param, data) {
			// 画像を受け取る ws version
			console.log("renderImage recieved", param, data);
			//this.emit(HiveCore.IMAGE_RECIEVED, param, data);
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

	testRender() {
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
			hcmd.CameraLookat("view",-72.64638509916728,23.40208319164952,171.44292740397938,0,0,0,0,1,0,60)
			hcmd.CameraClearColor("view",0,0,0,1)
			hcmd.CameraScreenSize("view",256,256)
			hcmd.CameraOutputFilename("view","view.jpg")
			hcmd.LoadOBJ("bunny_obj0","/home/khatta/bunny.obj","normal.frag")
			hcmd.SetModelTranslation("bunny_obj0",0,0,0,0,0,0,1,1,1)
			hcmd.SetModelShader("bunny_obj0","normal.frag")
			clearCache()

			hcmd.SetModelUniformFloat("bunny_obj0","power",1)
			hcmd.RenderCamera(256, 256, "view", "jpg", false)
			end)();
			`, function (err, data) {
				console.log("runScript end");
		});
	}

	changeNode() {
		console.log("change node at HiveCore");
	}
}
HiveCore.NODE_CHANGED = "core_node_changed";
HiveCore.IMAGE_RECIEVED = "core_image_revieved";