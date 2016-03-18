import Constants from "./Constants.jsx"
import NodeSystem from "../NodeSystem"
import {NodeIterator} from "./NodeIterator.jsx"

function uuid() {
    var uuid = "", i, random;
    for (i = 0; i < 32; i++) {
        random = Math.random() * 16 | 0;

        if (i == 8 || i == 12 || i == 16 || i == 20) {
            uuid += "_";
        }
        uuid += (i == 12 ? 4 : (i == 16 ? (random & 3 | 8) : random)).toString(16);
    }
    return uuid;
}

function positionGetter(target, nodeList){
    for(let i = 0, j = nodeList.length; i < j; ++i){
        if(Math.abs(nodeList[i].panel.pos[0] - target.panel.pos[0]) < 50 ||
           Math.abs(nodeList[i].panel.pos[1] - target.panel.pos[1]) < 50){
               target.panel.pos[0] += 50;
               target.panel.pos[1] += 50;
               positionGetter(target, nodeList);
               break;
           }
    }
    target.panel.zindex = nodeList.length + 1;
}


export default class ActionExecuter {
	constructor(store) {
		this.store = store;
        //
        // !!! don't have any data. !!!
        //

        // only Functions
        this.showConsoleOutput = this.showConsoleOutput.bind(this);
        this.setLayout = this.setLayout.bind(this);
		this.addNode = this.addNode.bind(this);
		this.exportSceneScript = this.exportSceneScript.bind(this);
        this.exportGroupNode = this.exportGroupNode.bind(this);
        this.importGroupNode = this.importGroupNode.bind(this);
        this.deleteNode = this.deleteNode.bind(this);
		this.deleteNodes = this.deleteNodes.bind(this);
		this.clearAll = this.clearAll.bind(this);
		this.changeNode = this.changeNode.bind(this);
		this.changeNodes = this.changeNodes.bind(this);
		this.changeNodeInput = this.changeNodeInput.bind(this);
		this.importNode = this.importNode.bind(this);
		this.addPlug = this.addPlug.bind(this);
		this.deletePlug = this.deletePlug.bind(this);
		this.changePanelVisible = this.changePanelVisible.bind(this);
		this.selectNode = this.selectNode.bind(this);
		this.unSelectNode = this.unSelectNode.bind(this);
		this.pasteNodes = this.pasteNodes.bind(this);
		this.getPlugsFromNodes = this.getPlugsFromNodes.bind(this);
		this.getGroupInoutFromNodes = this.getGroupInoutFromNodes.bind(this);
		this.copy = this.copy.bind(this);
		this.paste = this.paste.bind(this);
		this.delete = this.delete.bind(this);
		this.makeGroup = this.makeGroup.bind(this);
		this.unGroup = this.unGroup.bind(this);
		this.addGroup = this.addGroup.bind(this);
		this.digGroup = this.digGroup.bind(this);
		this.findGroupPath = this.findGroupPath.bind(this);
		this.publishOutput = this.publishOutput.bind(this);
		this.publishInput = this.publishInput.bind(this);
		this.unPublishInput = this.unPublishInput.bind(this);
		this.unPublishOutput = this.unPublishOutput.bind(this);
		this.deleteNodeRelatedValues = this.deleteNodeRelatedValues.bind(this);
		this.getChildNodes = this.getChildNodes.bind(this);
		this.getChildPlugs = this.getChildPlugs.bind(this);
		this.save = this.save.bind(this);
		this.load = this.load.bind(this);
		this.changeFrame = this.changeFrame.bind(this);
		this.addKeyFrame = this.addKeyFrame.bind(this);
		this.deleteKeyFrame = this.deleteKeyFrame.bind(this);
		this.applyCurrentFrame = this.applyCurrentFrame.bind(this);
		this.align = this.align.bind(this);
		this.alignNodes = this.alignNodes.bind(this);
		this.alignNode = this.alignNode.bind(this);
		this.createVarnameToNodeMap = this.createVarnameToNodeMap.bind(this);
	}

    /**
	 * アクションハンドラ
	 * @private
	 */
	actionHandler(payload) {
		if (payload && this.hasOwnProperty(payload.actionType)) {
			if (payload.hasOwnProperty("id") && payload.id === this.store.getDispatchToken()) {
				(() => {
					this[payload.actionType](payload);
				})();
			}
		}
	}

	/// nodeに初期値を設定.
	assignInitialNodeValue(node) {
		for (let key in ActionExecuter.initialData) {
			if (!node.hasOwnProperty(key)) {
				node[key] = JSON.parse(JSON.stringify(ActionExecuter.initialData[key]));
			}
		}
		if (!node.hasOwnProperty('uiFunc') || node.uiFunc === "") {
			// UI無し
			delete node.panel.visible;
		}
		for (let i = 0; i < node.input.length; i = i + 1) {
			if (!node.input[i].hasOwnProperty('nodeVarname')) {
				node.input[i].nodeVarname = node.varname;
				if (Array.isArray(node.input[i].array)) {
					let array = node.input[i].array;
					for (let n = 0; n < array.length; n = n + 1) {
						node.input[i].array[n].nodeVarname = node.varname;
					}
				}
			}
		}
		for (let i = 0; i < node.output.length; i = i + 1) {
			if (!node.output[i].hasOwnProperty('nodeVarname')) {
				node.output[i].nodeVarname = node.varname;
				if (Array.isArray(node.output[i].array)) {
					let array = node.output[i].array;
					for (let n = 0; n < array.length; n = n + 1) {
						node.output[i].array[n].nodeVarname = node.varname;
					}
				}
			}
		}
	}

    /**
	 * コンソール画面表示
	 */
    showConsoleOutput(payload) {
        if (payload.hasOwnProperty('show')) {
            this.store.emit(Constants.CONSOLEOUTPUT_SHOW, payload.show);
        }
    }

    /**
	 * レイアウト変更
	 */
    setLayout(payload) {
        if (payload.hasOwnProperty('mode')) {
            this.store.emit(Constants.LAYOUT_CHANGED, payload.mode);
        }
    }

	/**
	 * ノード追加
	 */
	addNode(payload) {
		if (payload.hasOwnProperty('nodeInfo')) {
			let node = null;
			if (payload.nodeInfo.hasOwnProperty('name')) {
				if (payload.nodeInfo.name === "Group") {
					node = payload.nodeInfo;
				} else {
					node = this.store.nodeCreator.CreateNodeInstance(payload.nodeInfo.name);
				}
			}
			if (payload.nodeInfo.hasOwnProperty('varname') && node.varname !== payload.nodeInfo.varname) {
				node.varname = payload.nodeInfo.varname;
			} else {
				if (payload.nodeInfo.name === "Group") {
					node.varname = "group_" + uuid();
				} else {
					node.varname = node.funcname + uuid();
				}
			}
			if (payload.nodeInfo.hasOwnProperty('node')) {
				node.node = payload.nodeInfo.node;
			}
			if (payload.nodeInfo.hasOwnProperty('panel')) {
				node.panel = payload.nodeInfo.panel;
				if (node.panel.zindex === 0) {
					node.panel.zindex = this.store.getNodes().length + 1;
				}
			}
			if (payload.nodeInfo.hasOwnProperty('input')) {
				node.input = payload.nodeInfo.input;
			}

			if (node) {
				this.assignInitialNodeValue(node);
				var nl = this.store.getNodes();
				positionGetter(node, nl);
				this.store.getNodes().push(node);
			}
			this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.getNodes().length);
			this.store.emit(Constants.NODE_ADDED, null, node);
		}
	}

	// 子ノードのうちgroupでないもののリストをdstに入れる。
	getChildNodes(dst, group) {
		let nodes = group.nodes;
		for (let i = 0; i < nodes.length; i = i + 1) {
			let n = nodes[i];
			if (this.store.isGroup(n)) {
				this.getChildNodes(dst, n);
			} else {
				dst.push(n);
			}
		}
	};

	// あるノード以下のplugsをdstに入れる
	getChildPlugs(dst, group) {
		let nodes = group.nodes;
		for (let i = 0; i < group.plugs.length; i = i + 1) {
			dst.push(group.plugs[i]);
		}
		for (let i = 0; i < nodes.length; i = i + 1) {
			let n = nodes[i];
			if (this.store.isGroup(n)) {
				this.getChildPlugs(dst, n);
			}
		}
	};

	/// ノードに参照をもっているplug, input, output, timelineを
	/// シーンから全て検索して削除する。
	deleteNodeRelatedValues(node) {
		// グループであれば、グループ以外の有効な子ノード全てに対して実行し直す。
		if (this.store.isGroup(node)) {
			let nodeList = [];
			this.getChildNodes(nodeList, node);
			for (let i = nodeList.length - 1; i >= 0; i = i - 1) {
				this.deleteNodeRelatedValues(nodeList[i]);
			}
		}

		// input, outputの削除.
		let tempPath = JSON.parse(JSON.stringify(this.store.data.nodePath));
		for (let i = tempPath.length; i >= 0; i = i - 1) {
			let inputs = this.store.getInput(tempPath);
			for (let k = inputs.length - 1; k >= 0; k = k - 1) {
				if (inputs[k].nodeVarname === node.varname) {
					inputs.splice(k, 1);
				}
			}
			let outputs = this.store.getOutput(tempPath);
			for (let k = outputs.length - 1; k >= 0; k = k - 1) {
				if (outputs[k].nodeVarname === node.varname) {
					outputs.splice(k, 1);
				}
			}
			if (i > 0) {
				tempPath.splice(i - 1, 1);
			}
		}

		// 関連するプラグを全部消す
		let origPath = this.store.data.nodePath;
		tempPath = JSON.parse(JSON.stringify(this.store.data.nodePath));
		for (let i = tempPath.length; i >= 0; i = i - 1) {
			let group = this.store.getDataAtPath(tempPath);
			let plugs = this.store.getPlugs(tempPath);
			this.store.data.nodePath = tempPath;
			for (let j = plugs.length - 1; j >= 0; j = j - 1) {
				let plug = plugs[j];
				if (plug.input.nodeVarname === node.varname) {
					this.deletePlug({ plugInfo : plug });
				} else if (plug.output.nodeVarname === node.varname) {
					this.deletePlug({ plugInfo : plug });
				}
			}
			if (this.store.isGroup(group)) {
				// グループから通常ノードに繋がっているプラグを削除.
				for (let k = group.input.length - 1; k >= 0; k = k - 1) {
					if (group.input[k].nodeVarname === node.varname) {
						group.input.splice(k, 1);
					}
				}
				for (let k = group.output.length - 1; k >= 0; k = k - 1) {
					if (group.output[k].nodeVarname === node.varname) {
						group.output.splice(k, 1);
					}
				}
			}
			if (i > 0) {
				tempPath.splice(i - 1, 1);
			}
		}

		// タイムラインのデータを全部消す。
		let timeData = this.store.data.timeline.data;
		if (timeData.hasOwnProperty('contents')) {
			for (let i = timeData.contents.length - 1; i >= 0; i = i - 1) {
				let content = timeData.contents[i];
				if (content.hasOwnProperty('nodeVarname') && content.nodeVarname === node.varname) {
					timeData.contents.splice(i, 1);
					break;
				}
			}
		}
		this.store.data.nodePath = origPath;
	}


    /**
	 * エクスポートSceneScript
	 */
	exportSceneScript(payload) {
        if (payload.hasOwnProperty('varname')) {
            let varname = payload.varname;
            let n;
            if (varname === "") { // ALL
                //n = this.store.getRootNodes();
                let nodeExe = new NodeSystem.NodeExecutor(this.store.data);
                let luasrc = "package.path = './?.lua;' .. package.path\n";
                luasrc    += "local BaseComponent = require('BaseComponent')\n";
                luasrc    += "local HIVE_ImageSaver = ImageSaver()\n";
                luasrc = luasrc + nodeExe.doNodes();
                //console.log('EXPORT>', luasrc);
          		var blob = new Blob([luasrc], {type: "text/plain"});
	        	saveAs(blob, "export.lua");
            } else { //
			    n = this.store.getNode(varname);
                if (n) {
                    console.log('NODE EXPORT', n);
                    let node = n.node;
                }
            }
		}
	}

    /**
	 * インポートグループノード
	 */
	importGroupNode(payload) {
	    if (payload.hasOwnProperty('data')) {
		    console.log('AAAA', payload.data);
            this.pasteNodes({nodeInfoList:[payload.data]});
        }
    }
    /**
	 * エクスポートグループノード
	 */
	exportGroupNode(payload) {
        if (payload.hasOwnProperty('varname')) {
            let varname = payload.varname;
            let outNode;
            if (varname === "") { // ALL
                outNode = this.store.data;
            } else { //
            console.log('exportGroupNode', varname);
			    outNode = this.store.findNode(this.store.data, varname);
            }

            if (outNode) {
                let grpdata = JSON.stringify(outNode, null, "    ");
                let blob = new Blob([grpdata], {type: "text/plain"});
              	saveAs(blob, "group.json");
            }
		}
	}

	/**
	 * ノード削除
	 */
	deleteNode(payload) {
		if (payload.hasOwnProperty('varname')) {
			let n = this.store.getNode(payload.varname);
			if (n) {
				this.store.getNodes().splice(n.index, 1);
				let node = n.node;
				this.deleteNodeRelatedValues(node);
				if (this.store.getNodes().length === 0) {
					console.log("TODO:グループ内に何も無くなった.");
				}

				this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.getNodes().length);
				this.store.emit(Constants.NODE_DELETED, null, n.node);
				this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
			}
		}
	}

	/**
	 * ノードを削除する
	 */
	deleteNodes(payload) {
		if (payload.hasOwnProperty('varnameList')) {
			for (let i = 0; i < payload.varnameList.length; i = i + 1) {
				this.deleteNode({varname : payload.varnameList[i].varname});
			}
		}
	}

	/**
	 * 現在のノード階層に対して入力を追加する.
	 */
	publishInput(payload) {
		if (payload.hasOwnProperty('data')) {
			let data = payload.data;
			if (data.hasOwnProperty('name') &&
				data.hasOwnProperty('nodeVarname') &&
				data.hasOwnProperty('type')) {
				let input = this.store.getInput();
				input.push(data);
				this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
			}
		}
	}

	/**
	 * 現在のノード階層に対して入力を削除する.
	 */
	unPublishInput(payload) {
		if (payload.hasOwnProperty('data')) {
			let data = payload.data;
			if (data.hasOwnProperty('name') &&
				data.hasOwnProperty('nodeVarname') &&
				data.hasOwnProperty('type')) {
				let input = this.store.getInput();

				let deleted = false;
				for (let i = 0; i < input.length; i = i + 1) {
					if (input[i].name === data.name && input[i].nodeVarname === data.nodeVarname) {
						input.splice(i, 1);
						// 関連するプラグを全部消す
						let tempPath = JSON.parse(JSON.stringify(this.store.data.nodePath));
						if (tempPath.length > 0) {
							tempPath.splice(tempPath.length-1, 1);
						}
						let plugs = this.store.getPlugs(tempPath);
						for (let k = plugs.length - 1; k >= 0; k = k - 1) {
							if (plugs[k].input.nodeVarname === data.nodeVarname && plugs[k].input.name === data.name) {
								plugs.splice(k, 1);
							}
						}
						deleted = true;
						break;
					}
				}
				if (deleted) {
					this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
				}
			}
		}
	}

	/**
	 * 現在のノード階層に対して出力を削除する.
	 */
	unPublishOutput(payload) {
		if (payload.hasOwnProperty('data')) {
			let data = payload.data;
			if (data.hasOwnProperty('name') &&
				data.hasOwnProperty('nodeVarname') &&
				data.hasOwnProperty('type')) {
				let output = this.store.getOutput();

				let deleted = false;
				for (let i = 0; i < output.length; i = i + 1) {
					if (output[i].name === data.name && output[i].nodeVarname === data.nodeVarname) {
						output.splice(i, 1);
						// 関連するプラグを全部消す
						let tempPath = JSON.parse(JSON.stringify(this.store.data.nodePath));
						if (tempPath.length > 0) {
							tempPath.splice(tempPath.length-1, 1);
						}
						let plugs = this.store.getPlugs(tempPath);
						for (let k = plugs.length - 1; k >= 0; k = k - 1) {
							if (plugs[k].output.nodeVarname === data.nodeVarname && plugs[k].output.name === data.name) {
								plugs.splice(k, 1);
							}
						}
						deleted = true;
						break;
					}
				}
				if (deleted) {
					this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
				}
			}
		}
	}

	/**
	 * 現在のノード階層に対して出力を追加する.
	 */
	publishOutput(payload) {
		if (payload.hasOwnProperty('data')) {
			let data = payload.data;
			if (data.hasOwnProperty('name') &&
				data.hasOwnProperty('nodeVarname') &&
				data.hasOwnProperty('type')) {
				let output = this.store.getOutput();
				output.push(data);
				this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
			}
		}
	}

	/**
	 * すべてのデータをクリア(削除）する
	 */
	clearAll(payload) {
	    let nodes = this.store.getNodes("");
        let plugs = this.store.getPlugs("");
        for (let i = plugs.length - 1; i >= 0; --i){
            this.deletePlug({
				plugInfo : plugs[i]
			});
        }
        for (let i = nodes.length - 1; i >= 0; --i){
            this.deleteNode({
				varname: nodes[i].varname
			});
        }
		this.store.data.name = "";    //シーン名
		this.store.data.varname = "Root";
		this.store.data.nodes = [];   // 全てのノード
		this.store.data.plugs = [];   // 全てのプラグ
		this.store.data.input = [];   // シーンの入力端子
		this.store.data.output = [];  // シーンの出力端子
		this.store.data.nodePath = []; // 表示しているノード階層のパス

		this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.getNodes().length);
		this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
	}

	/**
 	 * ノード変更.
 	 */
 	changeNode(payload) {
 		if (payload.hasOwnProperty('nodeInfo')) {
			let node = this.store.findNode(this.store.data, payload.nodeInfo.varname);
			if (node) {
				let dstNode = node;
				let srcNode =  payload.nodeInfo;

				let hasInput = srcNode.hasOwnProperty('input');
				let preInputs = JSON.stringify(dstNode.input);
				let postInputs = hasInput ? JSON.stringify(payload.nodeInfo.input) : null;

				let hasSelect = srcNode.hasOwnProperty('select');
				let preSelect = dstNode.select;
				let postSelect = hasSelect ? payload.nodeInfo.select : null;

				let hasPanel = srcNode.hasOwnProperty('panel');
				let prePanel = JSON.stringify(dstNode.panel);
				let postPanel = hasPanel ? JSON.stringify(payload.nodeInfo.panel) : null;
				let prePanelVisible = dstNode.panel.visible;
				let postPanelVisible = hasPanel ? payload.nodeInfo.panel.visible : null;
				let prePanelSize = JSON.stringify(dstNode.panel.size);
				let postPanelSize = hasPanel ? JSON.stringify(payload.nodeInfo.panel.size) : null;


				let hasNodeParam = srcNode.hasOwnProperty('node');
				let preNodePos = JSON.stringify(dstNode.node.pos);
				let postNodePos = hasNodeParam ? JSON.stringify(payload.nodeInfo.node.pos) : null;
				let preNodeClose = dstNode.node.close;
				let postNodeClose = hasNodeParam ? payload.nodeInfo.node.close : null;

				for (let info in payload.nodeInfo) {
					if (info !== "uiComponent" && node.hasOwnProperty(info)) {
						dstNode[info] = JSON.parse(JSON.stringify(payload.nodeInfo[info]));
					}
				}

				this.store.emit(Constants.NODE_CHANGED, null, dstNode);
				if (hasInput && preInputs !== postInputs) {
					if (this.store.isGroup(dstNode)) {
						// グループの入力が変更された場合は、入力に対応するノードの入力も変更する。
						let pre = JSON.parse(preInputs);
						let post = JSON.parse(postInputs);
						for (let k = 0; k < pre.length; k = k + 1) {
							if (JSON.stringify(pre[k].value) !== JSON.stringify(post[k].value)) {
								console.log(pre[k].nodeVarname);
								let target = this.store.findNode(dstNode, pre[k].nodeVarname);
								if (!target) {
									console.error("not found input node")
								}
								for (let m = 0; m < target.input.length; m = m + 1) {
									if (target.input[m].name === post[k].name) {
										target.input[m].value = post[k].value;
									}
								}
								this.store.emit(Constants.NODE_INPUT_CHANGED, null, target, -1);
							}
						}
					} else {
						this.store.emit(Constants.NODE_INPUT_CHANGED, null, dstNode);
					}
				}
				if (hasSelect && preSelect !== postSelect) {
					this.store.emit(Constants.NODE_SELECT_CHANGED, null, dstNode);
				}
				if (hasNodeParam && preNodePos !== postNodePos) {
					this.store.emit(Constants.NODE_POSITION_CHANGED, null, dstNode);
				}
				if (hasNodeParam && preNodeClose !== postNodeClose) {
					this.store.emit(Constants.NODE_CLOSE_CHANGED, null, dstNode);
				}
				if (hasPanel && prePanel !== postPanel) {
					this.store.emit(Constants.PANEL_CHANGED, null, dstNode);
				}
				if (hasPanel && prePanelVisible !== postPanelVisible) {
					this.store.emit(Constants.PANEL_VISIBLE_CHANGED, null, dstNode);
				}
				if (hasPanel && prePanelSize !== postPanelSize) {
					this.store.emit(Constants.PANEL_SIZE_CHANGED, null, dstNode);
				}
			}
 		}
 	}

	/**
	 * ノードの入力を変更する
	 */
	changeNodeInput(payload) {
		if (payload.hasOwnProperty('inputInfo')) {
			let info = payload.inputInfo;
			if (info.hasOwnProperty('varname') && info.hasOwnProperty('input')) {
				let node = this.store.findNode(this.store.data, info.varname);
				if (node) {
					let copy = JSON.parse(JSON.stringify(node.input));
					let isChanged = false;
					for (let i = 0; i < node.input.length; i = i + 1) {
						if (info.input.hasOwnProperty(node.input[i].name)) {
							copy[i].value = info.input[node.input[i].name];
							isChanged = true;
						}
					}
					if (isChanged) {
						this.changeNode({
							nodeInfo : {
								varname : info.varname,
								input : copy
							}
						});
					}
				}
			}
		}
	}

	/**
	 * ノードを複数変更する
	 */
	changeNodes(payload) {
 		if (payload.hasOwnProperty('nodeInfoList')) {
			for (let i = 0; i < payload.nodeInfoList.length; i = i + 1) {
				this.changeNode({
					nodeInfo : payload.nodeInfoList[i]
				});
			}
		}
	}

	// @private
	// 入力ノードの全てのnodevarnameからnodeへのマップを作り返す.
	// @param isRecursive trueの場合グループノードが含まれる場合は再帰的に作成する.
	createVarnameToNodeMap(varnameToNodes, nodes, isRecursive) {
		for (let i = 0; i <  nodes.length; i = i + 1) {
			varnameToNodes[nodes[i].varname] = nodes[i];
			if (this.store.isGroup(nodes[i])) {
				this.createVarnameToNodeMap(varnameToNodes, nodes[i].nodes);
			}
		}
	}

	/**
	 * ノード間のプラグリストを返す
	 */
	getPlugsFromNodes(nodes, plugs) {
		let resultPlugs = [];
		let varnameToNodes = {};
		let groups = [];
		this.createVarnameToNodeMap(varnameToNodes, nodes, true);

		for (let i = 0; i < plugs.length; i = i + 1) {
			let plug = plugs[i];
			let hasInput = varnameToNodes.hasOwnProperty(plug.input.nodeVarname);
			let hasOutput = varnameToNodes.hasOwnProperty(plug.output.nodeVarname);

			if (hasInput && hasOutput) {
				resultPlugs.push(plug);
			}
		}
		return resultPlugs;
	}

	/**
	 * ノードの集合から, ノードの集合外に繋がっている入力の、プラグリストを返す。
	 */
	getGroupInoutFromNodes(nodes, isInput) {
		let plugs = [];
		let varnameToNodes = {};
		this.createVarnameToNodeMap(varnameToNodes, nodes, true);

		for (let i = 0; i < this.store.getPlugs().length; i = i + 1) {
			let plug = this.store.getPlugs()[i];
			if (isInput &&
				varnameToNodes.hasOwnProperty(plug.input.nodeVarname) &&
				!varnameToNodes.hasOwnProperty(plug.output.nodeVarname)) {
				plugs.push(plug);
			}
			if (!isInput &&
				!varnameToNodes.hasOwnProperty(plug.input.nodeVarname) &&
				varnameToNodes.hasOwnProperty(plug.output.nodeVarname)) {
				plugs.push(plug);
			}
		}
		return plugs;
	}


	/**
	 * ペーストする.
	 */
	copy(payload) {
		this.store.emit(Constants.COPY_CALLED, null);
	}

	/**
	 * ペーストする.
	 */
	paste(payload) {
		this.store.emit(Constants.PASTE_CALLED, null);
	}

	/**
	 * 削除する
	 */
	delete(payload) {
		this.store.emit(Constants.DELETE_CALLED, null);
	}

	/**
	 * グループを追加する.
	 */
	addGroup(payload) {
		if (payload.hasOwnProperty('group')) {
			let group = payload.group;
			let nodes = group.nodes;
			// まずnodesを取り除く（deleteはしない)
			// plugは変更なし。
			for (let k = 0; k < nodes.length; k = k + 1) {
				let n = nodes[k];
				for (let i = this.store.getNodes().length - 1; i >= 0; i = i - 1) {
					let dn = this.store.getNodes()[i];
					if (n.varname === dn.varname) {
						this.store.getNodes().splice(i, 1);
					}
				}
			}
			let plugs = group.plugs;
			for (let k = 0; k < plugs.length; k = k + 1) {
				let p = plugs[k];
				for (let i = this.store.getPlugs().length - 1; i >= 0; i = i - 1) {
					let pp = this.store.getPlugs()[i];
					if (p.input.nodeVarname === pp.input.nodeVarname &&
						p.output.nodeVarname === pp.output.nodeVarname &&
						p.input.name === pp.input.name&&
						p.output.name === pp.output.name) {
						this.store.getPlugs().splice(i, 1);
					}
				}
			}
			// groupを追加する。
			console.log("addgroup", group)
			this.addNode({ nodeInfo : group });

			//this.store.data.nodePath = [group.varname];

			this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.getNodes().length);
			this.store.emit(Constants.NODE_ADDED, null, group);
			this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
		}
	}

	/**
	 * グループを作成する
	 */
	makeGroup(payload) {
		let nodeList = this.store.getSelectedNodeList();
		if (nodeList.length <= 1) { return; }

		console.log("nodeList", nodeList)

		// ノードリストから外部に出ているプラグを取得.
		let inplugs = this.getGroupInoutFromNodes(nodeList, true);
		let outplugs = this.getGroupInoutFromNodes(nodeList, false);
		let inputs = [];
		let outputs = [];

		for (let i = 0; i < nodeList.length; i = i + 1) {
			let n = nodeList[i];
			for (let k = 0; k < inplugs.length; k = k + 1) {
				let plug = inplugs[k];
				if (this.store.findNode(n, plug.input.nodeVarname)) {
					let inputIterator = NodeIterator.makeInputIterator(this.store, n);
					for (let v of inputIterator) {
						if (v.input && v.input.name === plug.input.name) {
							inputs.push(v.input);
						}
					}
				}
			}
			for (let k = 0; k < outplugs.length; k = k + 1) {
				let plug = outplugs[k];
				if (this.store.findNode(n, plug.output.nodeVarname)) {
					let outputIterator = NodeIterator.makeOutputIterator(this.store, n);
					for (let v of outputIterator) {
						if (v.output && v.output.name === plug.output.name) {
							outputs.push(v.output);
						}
					}
				}
			}
		}

		// 外部に公開している端子の処理
		let ins = this.store.getInput();
		let outs = this.store.getOutput();
		let varnameToInput = {};
		let varnameToOutput = {};
		for (let i = 0; i < ins.length; i = i + 1) {
			varnameToInput[ins[i].nodeVarname] = ins[i];
		}
		for (let i = 0; i < outs.length; i = i + 1) {
			varnameToOutput[outs[i].nodeVarname] = outs[i];
		}
		for (let i = 0; i < nodeList.length; i = i + 1) {
			let inoutIterator = NodeIterator.makeInputOutputIterator(this.store, nodeList[i]);
			for (let v of inoutIterator) {
				if (v.input && varnameToInput.hasOwnProperty(v.input.nodeVarname)) {
					if (varnameToInput[v.input.nodeVarname].name === v.input.name) {
						inputs.push(v.input);
					}
				}
				if (v.output && varnameToOutput.hasOwnProperty(v.output.nodeVarname)) {
					if (varnameToOutput[v.output.nodeVarname].name === v.output.name) {
						outputs.push(v.output);
					}
				}
			}
		}

		let group = {
			name : "Group",
			varname : "group_" + uuid(),
			nodes : nodeList,
			plugs : this.getPlugsFromNodes(nodeList, this.store.getPlugs()),
			input : inputs,
			output : outputs
		};
		this.addGroup({ group : group });
		this.store.emit(Constants.MAKE_GROUP_CALLED, null);
	}

	/**
	 * グループを解除する
	 */
	unGroup(payload) {
		let nodeList = this.store.getSelectedNodeList();
		let groupList = [];
		for (let i = 0; i < nodeList.length; i = i + 1) {
			if (this.store.isGroup(nodeList[i])) {
				groupList.push(nodeList[i]);
			}
		}
		for (let k = 0; k < groupList.length; k = k + 1) {
			// グループノードが保持しているノードとプラグを現在の階層に追加.
			let group = groupList[k];
			let nodes = this.store.getNodes();
			let plugs = this.store.getPlugs();
			for (let i = 0; i < group.nodes.length; i = i + 1) {
				nodes.push(group.nodes[i]);
			}
			for (let i = 0; i < group.plugs.length; i = i + 1) {
				plugs.push(group.plugs[i]);
			}
			// グループノードを削除.
			for (let i = 0; i < nodes.length; i = i + 1) {
				if (nodes[i].varname === group.varname) {
					let node = nodes[i];
					nodes.splice(i, 1);
					this.store.emit(Constants.NODE_DELETED, null, node);
					break;
				}
			}
			this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.getNodes().length);
			this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
		}
	}

	/// グループのパスを探して返す
	findGroupPath(root, varname, groupPath) {
		if (!root) {
			return null;
		}
		let nameList = [].concat(groupPath);
		if (this.store.isGroup(root)) {
			nameList.push(root.varname);
			if (varname === root.varname) {
				return nameList;
			}
			for (let i = 0; i < root.nodes.length; i = i + 1) {
				let list = this.findGroupPath(root.nodes[i], varname, nameList);
				if (list) {
					return list;
				}
			}
		}
		return null;
	}

	/**
	 * グループを移動する
	 */
	digGroup(payload) {
		if (payload.hasOwnProperty('groupVarname')) {
			let groupPath = this.findGroupPath(this.store.data, payload.groupVarname, []);
			if (groupPath && groupPath.length > 0) {
				groupPath.splice(0, 1);
				this.store.data.nodePath = groupPath;
				this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.getNodes().length);
				this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
			}
		}
	}

	/**
	 * ファイルに保存する
	 */
	save(payload) {
		var data = this.store.data;
		var blob = new Blob([JSON.stringify(data, null, 2)], {type: "text/plain;charset=utf-8"});
		saveAs(blob, "save.json");
	}

	/**
	 * データから読み込む
	 */
	load(payload) {
		if (payload.hasOwnProperty('data')) {
			this.clearAll();
			// this.store.dataはnodesystemが参照しているため代入してはいけない.
			let data = payload.data;
			for (let i in data) {
				if (data.hasOwnProperty(i)) {
					this.store.data[i] = data[i];
				}
			}
			// すべての有効なノードとプラグのリストを取得
			let nodeList = [];
			this.getChildNodes(nodeList, this.store.data);
			let plugList = [];
			this.getChildPlugs(plugList, this.store.data);

			// 追加された通知を送る
			for (let i = 0 ; i < nodeList.length; i = i + 1) {
				let n = nodeList[i];
				this.store.emit(Constants.NODE_ADDED, null, n);
			}
			this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.getNodes().length);
			for (let i = 0; i < plugList.length; i = i + 1) {
				let p = plugList[i];
				this.store.emit(Constants.PLUG_ADDED, null, p);
			}
			this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
		}
	}

	// @private
	makeVarnameConvertTable(convertTable, node) {
		convertTable[node.varname] = node.funcname + uuid();
		if (this.store.isGroup(node)) {
			for (let i = 0; i < node.nodes.length; i = i + 1) {
				this.makeVarnameConvertTable(convertTable, node.nodes[i]);
			}
		}
	}

	// @private
	pasteNode(convertTable, node, plugs) {
		const oldVarname = node.varname;

		// varname変更.
		node.varname = convertTable[node.varname];

		// input, outputのvarname変更.
		let inputIterator = NodeIterator.makeInputIterator(this.store, node);
		for (let v of inputIterator) {
			if (v.input && convertTable.hasOwnProperty(v.input.nodeVarname)) {
				v.input.nodeVarname = convertTable[v.input.nodeVarname];
			}
		}
		for (let i = 0; i < node.output.length; i = i + 1) {
			if (convertTable.hasOwnProperty(node.output[i].nodeVarname)) {
				node.output[i].nodeVarname = convertTable[node.output[i].nodeVarname];
			}
		}

		// プラグの接続を新規に作ったノードに変更.
		for (let i = 0; i < plugs.length; i = i + 1) {
			if (convertTable.hasOwnProperty(plugs[i].input.nodeVarname)) {
				plugs[i].input.nodeVarname = convertTable[plugs[i].input.nodeVarname];
			}
			if (convertTable.hasOwnProperty(plugs[i].output.nodeVarname)) {
				plugs[i].output.nodeVarname = convertTable[plugs[i].output.nodeVarname];
			}
		}

		// タイムラインの複製
		let content = this.store.getTimelineContent(oldVarname);
		if (content) {
			content = JSON.parse(JSON.stringify(content));
			content.nodeVarname = node.varname;
			for (let i = 0; i < content.props.length; i = i + 1) {
				content.props[i].nodeVarname = convertTable[content.props[i].nodeVarname];
			}
			this.store.data.timeline.data.contents.push(content);
		}

		// グループの場合再帰.
		if (this.store.isGroup(node)) {
			let newplugs = JSON.parse(JSON.stringify(this.getPlugsFromNodes(node.nodes, node.plugs)));
			let merged = [].concat(plugs).concat(newplugs);
			for (let i = 0; i < node.nodes.length; i = i + 1) {
				this.pasteNode.bind(this)(convertTable, node.nodes[i], merged);
			}
			node.plugs = newplugs;
		}
	}

	/**
	 * ノードをペーストする.
	 */
	pasteNodes(payload) {
 		if (payload.hasOwnProperty('nodeInfoList')) {
			// ノード間のプラグリストのコピーを取得.
			let plugs = JSON.parse(JSON.stringify(this.getPlugsFromNodes(payload.nodeInfoList, this.store.getPlugs())));

			let convertTable = {};
			for (let i = 0; i < payload.nodeInfoList.length; i = i + 1) {
				let src = payload.nodeInfoList[i];
				this.makeVarnameConvertTable(convertTable, src);
			}

			// ノードを追加
			for (let i = 0; i < payload.nodeInfoList.length; i = i + 1) {
				let src = payload.nodeInfoList[i];

				// varname書き換え, タイムラインのペースト.
				this.pasteNode.bind(this)(convertTable, src, plugs);

				// 最上位階層はパネルの位置を設定する必要がある
				// また,1回はaddNodeしてnodesystemを更新する必要がある
				this.addNode({ nodeInfo : src });
			}

			// 最上位階層のプラグを追加
			for (let i = 0; i < plugs.length; i = i + 1) {
				this.addPlug({ plugInfo: plugs[i] });
			}
		}
	}

	/**
	 * ノードを選択する
	 */
	selectNode(payload) {
		if (payload.hasOwnProperty('nodeVarnameList')) {
			for (let i = 0; i < payload.nodeVarnameList.length; i = i + 1) {
				let n = this.store.getNode(payload.nodeVarnameList[i]);
				if (n && !n.nodeselect) {
					n.node.select = true;
					this.store.emit(Constants.NODE_SELECT_CHANGED, null, n.node, n.index);
				}
			}
		}
	}

	/**
	 * ノードの選択を解除する
	 */
	unSelectNode(payload) {
		if (payload.hasOwnProperty('nodeVarnameList')) {
			if (payload.nodeVarnameList.length > 0) {
				for (let i = 0; i < payload.nodeVarnameList.length; i = i + 1) {
					let n = this.store.getNode(payload.nodeVarnameList[i]);
					if (n.node.select) {
						if (!payload.hasOwnProperty('excludeVarname') || payload.excludeVarname !== n.nodevarname) {
							n.node.select = false;
							this.store.emit(Constants.NODE_SELECT_CHANGED, null, n.node, n.index);
						}
					}
				}
			} else {
				for (let i = 0; i < this.store.getNodes().length; i = i + 1) {
					// if (this.store.getNodes()[i].select) {
						this.store.getNodes()[i].select = false;
						this.store.emit(Constants.NODE_SELECT_CHANGED, null, this.store.getNodes()[i], i);
					// }
				}
			}
		}
	}

	/**
	 * ノードを整列する
	 */
	align(payload) {
		this.store.emit(Constants.NODE_ALIGN_CALLED, null);
	}

	// private
	alignNode(varnameToNode, inputToParentNode, depthToPos, nodeSizes, aligned, varname, pos, depth) {
		if (!varnameToNode.hasOwnProperty(varname)) { return; }
		const bound = nodeSizes[varname];
		if (!aligned.hasOwnProperty(varname)) {
			const node = varnameToNode[varname];
			if (!depthToPos.hasOwnProperty(depth)) {
				const offset = { x : (depth > 0) ? -bound.w - 50 : 0, y : (depth > 0) ? bound.h + 50 : 0 };
				depthToPos[depth] = { x : pos.x + offset.x, y : pos.y, w : bound.w, h : bound.h};
			} else {
				const p = depthToPos[depth];
				const offset = { x : -p.w - 50, y : p.h + 30 };
				depthToPos[depth] = { x : p.x, y : p.y + offset.y, w : bound.w, h : bound.h };
			}
			pos.x = depthToPos[depth].x;
			pos.y = depthToPos[depth].y;

			let n = JSON.parse(JSON.stringify(node.node));
			n.pos[0] = pos.x;
			n.pos[1] = pos.y;
			this.changeNode({
				nodeInfo : {
					varname : varname,
					node : n
				}
			});
			aligned[varname] = bound;

			// 出力端子から繋がっているノードがあるか調べる.
			let plugs = this.store.getPlugs();
			let hasOutputConnection = false;
			for (let i = 0; i < node.output.length; i = i + 1) {
				for (let k = 0; k < plugs.length; k = k + 1) {
					if (plugs[k].output.nodeVarname === node.output[i].nodeVarname) {
						hasOutputConnection = true;
						break;
					}
				}
			}
			if (!hasOutputConnection) {
				// 葉ノードであるので、depthToPosをリセット
				let temp = depthToPos[depth];
				depthToPos = {};
				depthToPos[depth] = temp;
				console.log("depthToPos")
			}

			// 入力端子から親ノードへ再帰.
			let inputIterator = NodeIterator.makeInputIterator(this.store, node);
			for (let v of inputIterator) {
				if (v.input) {
					const key = v.input.nodeVarname + "_" + v.input.name;
					if (inputToParentNode.hasOwnProperty(key)) {
						let parentNode = inputToParentNode[key];
						if (parentNode !== undefined) {
							this.alignNode(varnameToNode, inputToParentNode, depthToPos, nodeSizes, aligned, parentNode.varname, pos, depth + 1);
						}
					}
				}
			}
		}
	}

	/**
	 * ノードを整列させる
	 */
	alignNodes(payload) {
		if (payload.hasOwnProperty('zoom') &&
			payload.hasOwnProperty('rect') &&
			payload.hasOwnProperty('realRect') &&
			payload.hasOwnProperty('nodeSizes')) {

			console.log("alignNodes", payload.zoom, payload.rect, payload.realRect, payload.nodeSizes);

			const zoom = payload.zoom;
			const rect = payload.rect;
			const realRect = payload.realRect;
			const nodeSizes = payload.nodeSizes;

			const varnameToOrder = this.store.nodeExecutor.getOrderByVarname(Object.keys(payload.nodeSizes));
			let orderToVarname = {};
			for (let varname in varnameToOrder) {
				orderToVarname[varnameToOrder[varname]] = varname;
			}

			let varnameToNode = {};
			let plugs = this.store.getPlugs();
			for (let varname in nodeSizes) {
				varnameToNode[varname] = this.store.findNode(this.store.getRootNode(), varname);
			}

			let inputToParentNode = {};
			for (let varname in varnameToNode) {
				let node = varnameToNode[varname];
				for (let k = 0; k < plugs.length; k = k + 1) {
					const inVarname  = plugs[k].input.nodeVarname;
					const inName = plugs[k].input.name;
					const outVarname = plugs[k].output.nodeVarname;
					let inputIterator = NodeIterator.makeInputIterator(this.store, node);
					for (let v of inputIterator) {
						if (v.input && v.input.nodeVarname === inVarname && v.input.name === inName) {
							const key = v.input.nodeVarname + "_" + v.input.name;
							if (varnameToNode.hasOwnProperty(outVarname)) {
								inputToParentNode[key] = varnameToNode[outVarname];
							} else {
								for (let vn in varnameToNode) {
									if (this.store.findNode(varnameToNode[vn], outVarname)) {
										inputToParentNode[key] = varnameToNode[vn];
									}
								}
							}
						}
					}
				}
			}

			let aligned = {};
			let depthToPos = {};
			const orders = Object.keys(orderToVarname);
			for (let i = orders.length - 1; i >= 0; i = i - 1) {
				const order = orders[i];
				const varname = orderToVarname[order];
				const bound = nodeSizes[varname];
				this.alignNode(varnameToNode, inputToParentNode, depthToPos, nodeSizes, aligned, varname, bound, 0);
			}
		}
	};

	/**
	 * ノードインポート
	 */
	importNode(payload) {
		if (payload.hasOwnProperty('nodeInfo')) {
            this.store.getNodes().push(payload.nodeInfo);
			this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.getNodes().length);
			this.store.emit(Constants.NODE_ADDED, null, payload.nodeInfo);
		}
	}

	/**
	 * プラグを追加
	 */
	addPlug(payload) {
		if (payload.hasOwnProperty('plugInfo')) {
			for (let i = 0; i < this.store.getPlugs().length; i = i + 1) {
				if (this.store.getPlugs()[i].input.nodeVarname === payload.plugInfo.input.nodeVarname &&
					this.store.getPlugs()[i].input.name === payload.plugInfo.input.name) {
					console.log("既に繋がっている入力端子に繋げようとした");
					return;
				}
			}
			if (payload.plugInfo.input.nodeVarname === payload.plugInfo.output.nodeVarname) {
				console.log("同じノードに繋げようとした");
				return;
			}
			this.store.getPlugs().push(payload.plugInfo);
			this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
			this.store.emit(Constants.PLUG_ADDED, null, payload.plugInfo);
		}
	}

	/**
	 * プラグ削除
	 */
	deletePlug(payload) {
		if (payload.hasOwnProperty('plugInfo')) {
			let info = payload.plugInfo;
			for (let i = 0; i < this.store.getPlugs().length; i = i + 1) {
				let input = this.store.getPlugs()[i].input;
				let output = this.store.getPlugs()[i].output;
				if (output.nodeVarname === info.output.nodeVarname &&
					input.nodeVarname === info.input.nodeVarname &&
					output.name === info.output.name) {

					if (Array.isArray(input.array)) {
						for (let k = 0; k < input.array.length; k = k + 1) {
							if (input.array[k].name === info.input.name) {
								let plug = this.store.getPlugs()[i];
								this.store.getPlugs().splice(i, 1);
								this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
								this.store.emit(Constants.PLUG_DELETED, null, plug);
								return;
							}
						}
					} else if (input.name === info.input.name) {
						let plug = this.store.getPlugs()[i];
						this.store.getPlugs().splice(i, 1);
						this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
						this.store.emit(Constants.PLUG_DELETED, null, plug);
						return;
					}
				}
			}
		}
	}

	/**
	 * パネルの表示状態を切り替える
	 */
	changePanelVisible(payload) {
		if (payload.hasOwnProperty('varname')) {
			let n = this.store.getNode(payload.varname);
			if (n) {
				let node = n.node;
				let index = n.index;
				node.panel.visible = payload.visible;
				this.store.emit(Constants.NODE_CHANGED, null, node, index);
				this.store.emit(Constants.PANEL_CHANGED, null, node, index);
				this.store.emit(Constants.PANEL_VISIBLE_CHANGED, null, node, index);
			}
        }
    }

	/// フレーム番号から現在の補間値を求めて適用する.
	applyCurrentFrame(frame) {
		let data = this.store.data.timeline.data;
		if (!data.hasOwnProperty('contents')) { return; }
		for (let i = 0; i < data.contents.length; i = i + 1) {
			// 1content = 1nodeに対応.
			let content = data.contents[i];
			for (let k = 0; k < content.props.length; k = k + 1) {
				let prop = content.props[k];
				// 前後のキーを検索.
				let preKey = null;
				let postKey = null;
				for (let key in prop.data) {
					if (frame < Number(key)) {
						postKey = Number(key);
						break;
					}
					preKey = Number(key);
				}
				if (!preKey) { preKey = postKey; }
				if (!postKey) { postKey = preKey; }
				if (preKey && postKey) {
					let value = prop.data[preKey];
					if (preKey < postKey && (typeof value !== "string") && (typeof value !== "boolean")) {
						// 線形補間.
						value = lerp(prop.data[preKey], prop.data[postKey], (frame - preKey) / (postKey - preKey));
					}
					let inputInfo = {};
					inputInfo.varname = prop.nodeVarname;
					inputInfo.input = {};
					inputInfo.input[prop.name] = value;
					this.changeNodeInput({
						inputInfo : inputInfo
					});
					//console.log("pre post", preKey, postKey, inputInfo);
					this.store.emit(Constants.CURRENT_FRAME_APPLIED, null, content, prop);
				}
			}
		}
	}

	/**
	 * フレームを変更する.
	 */
	changeFrame(payload) {
		if (payload.hasOwnProperty("frame")) {
			if (payload.frame < 0) {
				payload.frame = 0;
			}
			let f = Math.floor(payload.frame + 0.5);

			// ここで値の補間を行う.
			if (this.store.data.timeline.frame !== f) {
				this.store.data.timeline.frame = f;
				this.applyCurrentFrame(f);
			}
			this.store.emit(Constants.CHANGE_FRAME, null, f);
		}
	}

	/**
	 * キーフレームを追加する
	 * @param frame フレーム番号
	 * @param node ノード
	 * @param input キーフレームを登録する入力
	 */
	addKeyFrame(payload) {
		if (payload.hasOwnProperty("frame") && payload.hasOwnProperty("node") && payload.hasOwnProperty("input")) {
			let node = payload.node;
			let frame = payload.frame;
			let input = payload.input;

			let data = this.store.data.timeline.data;
			if (!data.hasOwnProperty('contents')) {
				data.contents = [];
			}
			let varnameToContent = {};
			for (let i = 0; i < data.contents.length; i = i + 1) {
				if (data.contents[i].hasOwnProperty('nodeVarname')) {
					varnameToContent[data.contents[i].nodeVarname] = data.contents[i];
				}
			}
			let content;
			if (!varnameToContent.hasOwnProperty(input.nodeVarname)) {
				content = {
					name: node.label ? node.label : node.name,
					nodeVarname : input.nodeVarname,
					color: "rgb(32, 96, 196)",
					propColor: "rgba(8, 62, 162, 1.0)",
					props: []
				};
				data.contents.push(content);
			} else {
			 	content = varnameToContent[input.nodeVarname];
			}
			let prop = null;
			for (let i = 0; i < content.props.length; i = i + 1) {
				if (content.props[i].name === input.name &&
					content.props[i].nodeVarname === input.nodeVarname) {
					prop = content.props[i];
					break;
				}
			}
			if (!prop) {
				prop = {
					name: input.name,
					nodeVarname : input.nodeVarname,
					data: {}
				};
				content.props.push(prop);
			}
			prop.data[frame] = JSON.parse(JSON.stringify(payload.input.value));

			this.store.emit(Constants.KEYFRAME_ADDED, null, content);
		}
	}

	/**
	 * キーフレームを削除する
	 */
	deleteKeyFrame(payload) {
		if (payload.hasOwnProperty("frame") && payload.hasOwnProperty("node") && payload.hasOwnProperty("input")) {
			let node = payload.node;
			let frame = payload.frame;
			let input = payload.input;

			let data = this.store.data.timeline.data;
			if (!data.hasOwnProperty('contents')) {
				console.error("not found keyframe for delete");
			}
			let varnameToContent = {};
			for (let i = 0; i < data.contents.length; i = i + 1) {
				if (data.contents[i].hasOwnProperty('nodeVarname')) {
					varnameToContent[data.contents[i].nodeVarname] = data.contents[i];
				}
			}
			if (!varnameToContent.hasOwnProperty(input.nodeVarname)) {
				console.error("not found keyframe for delete");
			}
			let content = varnameToContent[input.nodeVarname];
			for (let i = 0; i < content.props.length; i = i + 1) {
				if (content.props[i].name === input.name &&
					content.props[i].nodeVarname === input.nodeVarname) {
					if (content.props[i].data.hasOwnProperty(frame)) {
						delete content.props[i].data[frame];
						this.applyCurrentFrame(this.store.getCurrentFrame());
						this.store.emit(Constants.KEYFRAME_DELETED, null, content);
					}
					return;
				}
			}
			console.error("not found keyframe for delete");
		}
	}
}

ActionExecuter.initialData = {
	"varname": "invalid",
	"label" : "",
	"select" : false,
	"node" : {
		"close" : false,
		"pos": [100, 100],
		"zindex": 0,
	},
	"panel": {
		"visible": true,
		"size": [100, 100],
		"pos": [100, 100],
		"zindex": 0
	}
};
