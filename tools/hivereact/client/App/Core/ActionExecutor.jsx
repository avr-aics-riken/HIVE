import Constants from "./Constants.jsx"

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
		this.addGroup = this.addGroup.bind(this);
		this.digGroup = this.digGroup.bind(this);
		this.findGroupPath = this.findGroupPath.bind(this);
		this.publishOutput = this.publishOutput.bind(this);
		this.publishInput = this.publishInput.bind(this);
		this.unPublishInput = this.unPublishInput.bind(this);
		this.unPublishOutput = this.unPublishOutput.bind(this);
		this.deleteNodeRelatedValues = this.deleteNodeRelatedValues.bind(this);
		this.save = this.save.bind(this);
		this.load = this.load.bind(this);
	}

    /**
	 * アクションハンドラ
	 * @private
	 */
	actionHandler(payload) {
		if (payload && this.hasOwnProperty(payload.actionType)) {
			if (payload.hasOwnProperty("id") && payload.id === this.store.getDispatchToken()) {
				(() => {
					this[payload.actionType].bind(this)(payload);
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
					node = this.store.nodeSystem.CreateNodeInstance(payload.nodeInfo.name);
				}
			}
			if (payload.nodeInfo.hasOwnProperty('varname') && node.varname !== payload.nodeInfo.varname) {
				node.varname = payload.nodeInfo.varname;
			} else {
				node.varname = node.funcname + uuid();
			}
			if (payload.nodeInfo.hasOwnProperty('node')) {
				node.node = payload.nodeInfo.node;
			}
			if (payload.nodeInfo.hasOwnProperty('panel')) {
				if (payload.nodeInfo.panel.zindex === 0) {
					payload.nodeInfo.panel.zindex = this.store.getNodes().length + 1;
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

	/// ノードに参照をもっているplug, input, outputを
	/// シーンから全て検索して削除する。
	deleteNodeRelatedValues(node) {
		// 関連するプラグを削除.
		for (let i = this.store.getPlugs().length - 1; i >= 0; i = i - 1) {
			let plug = this.store.getPlugs()[i];
			if (plug.input.nodeVarname === node.varname) {
				this.deletePlug({ plugInfo : plug });
			} else if (plug.output.nodeVarname === node.varname) {
				this.deletePlug({ plugInfo : plug });
			}
			if (this.store.isGroup(node)) {
				// グループから通常ノードに繋がっているプラグを削除.
				for (let k = 0; k < node.input.length; k = k + 1) {
					if (node.input[k].nodeVarname === plug.input.nodeVarname) {
						this.deletePlug({ plugInfo : plug });
					}
				}
				for (let k = 0; k < node.output.length; k = k + 1) {
					if (node.output[k].nodeVarname === plug.output.nodeVarname) {
						this.deletePlug({ plugInfo : plug });
					}
				}
			}
		}
		// グループの公開端子を削除.
		// TODO: グループのグループ
		let inputs = this.store.getInput();
		for (let k = inputs.length - 1; k >= 0; k = k - 1) {
			if (inputs[k].nodeVarname === node.varname) {
				inputs.splice(k, 1);
			}
		}
		let outputs = this.store.getOutput();
		for (let k = outputs.length - 1; k >= 0; k = k - 1) {
			if (outputs[k].nodeVarname === node.varname) {
				outputs.splice(k, 1);
			}
		}
		// 関連するプラグを全部消す
		let tempPath = JSON.parse(JSON.stringify(this.store.data.nodePath));
		if (tempPath.length > 0) {
			tempPath.splice(tempPath.length-1, 1);
		}
		let plugs = this.store.getPlugs(tempPath);
		for (let k = plugs.length - 1; k >= 0; k = k - 1) {
			if (plugs[k].input.nodeVarname === node.varname || plugs[k].output.nodeVarname === node.varname) {
				plugs.splice(k, 1);
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
            this.action.deleteNode({
				varname: nodes[i].varname
			});
        }
		this.store.data.nodePath = [];
		this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.getNodes().length);
		this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
	}

	/**
 	 * ノード変更.
 	 */
 	changeNode(payload) {
 		if (payload.hasOwnProperty('nodeInfo')) {
 			for (let i = 0; i < this.store.getNodes().length; i = i + 1) {
				if (this.store.getNodes()[i].varname === payload.nodeInfo.varname) {
					let dstNode = this.store.getNodes()[i];
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

					for (let info in payload.nodeInfo) {
						if (info !== "uiComponent" && this.store.getNodes()[i].hasOwnProperty(info)) {
							this.store.getNodes()[i][info] = JSON.parse(JSON.stringify(payload.nodeInfo[info]));
						}
					}

					this.store.emit(Constants.NODE_CHANGED, null, this.store.getNodes()[i], i);
					if (hasInput && preInputs !== postInputs) {
						this.store.emit(Constants.NODE_INPUT_CHANGED, null, this.store.getNodes()[i], i);
					}
					if (hasSelect && preSelect !== postSelect) {
						this.store.emit(Constants.NODE_SELECTE_CHANGED, null, this.store.getNodes()[i], i);
					}
					if (hasPanel && prePanel !== postPanel) {
						this.store.emit(Constants.PANEL_CHANGED, null, this.store.getNodes()[i], i);
					}
					if (hasPanel && prePanelVisible !== postPanelVisible) {
						this.store.emit(Constants.PANEL_VISIBLE_CHANGED, null, this.store.getNodes()[i], i);
					}
					if (hasPanel && prePanelSize !== postPanelSize) {
						this.store.emit(Constants.PANEL_SIZE_CHANGED, null, this.store.getNodes()[i], i);
					}
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
				let n = this.store.getNode(info.varname);
				if (n) {
					let node = n.node;
					for (let i = 0; i < node.input.length; i = i + 1) {
						if (info.input.hasOwnProperty(node.input[i].name)) {
							let copy = JSON.parse(JSON.stringify(node.input));
							copy[i].value = info.input[node.input[i].name];
							this.changeNode({
								nodeInfo : {
									varname : info.varname,
									input : copy
								}
							})
						}
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


	/**
	 * ノード間のプラグリストを返す
	 */
	getPlugsFromNodes(nodes) {
		let plugs = [];
		let varnameToNodes = {};
		for (let i = 0; i <  nodes.length; i = i + 1) {
			varnameToNodes[nodes[i].varname] = nodes[i];
		}

		for (let i = 0; i < this.store.getPlugs().length; i = i + 1) {
			let plug = this.store.getPlugs()[i];
			if (varnameToNodes.hasOwnProperty(plug.input.nodeVarname) &&
				varnameToNodes.hasOwnProperty(plug.output.nodeVarname)) {
				plugs.push(plug);
			}
		}
		return plugs;
	}


	/**
	 * ノードの集合から, ノードの集合外に繋がっている入力の、プラグリストを返す。
	 */
	getGroupInoutFromNodes(nodes, isInput) {
		let plugs = [];
		let varnameToNodes = {};
		for (let i = 0; i <  nodes.length; i = i + 1) {
			varnameToNodes[nodes[i].varname] = nodes[i];
		}

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

			console.log("addGroup");
		}
	}

	/**
	 * グループを作成する
	 */
	makeGroup(payload) {
		let nodeList = this.store.getSelectedNodeList();
		if (nodeList.length <= 1) { return; }

		console.log("nodeList", nodeList)

		let inplugs = this.getGroupInoutFromNodes(nodeList, true);
		let outplugs = this.getGroupInoutFromNodes(nodeList, false);
		let inputs = [];
		let outputs = [];
		for (let i = 0; i < nodeList.length; i = i + 1) {
			let n = nodeList[i];
			for (let k = 0; k < inplugs.length; k = k + 1) {
				let plug = inplugs[k];
				if (n.varname === plug.input.nodeVarname) {
					for (let j = 0; j < n.input.length; j = j + 1) {
						if (n.input[j].name === plug.input.name) {
							inputs.push(n.input[j]);
						}
					}
				}
			}
			for (let k = 0; k < outplugs.length; k = k + 1) {
				let plug = outplugs[k];
				if (n.varname === plug.output.nodeVarname) {
					for (let j = 0; j < n.output.length; j = j + 1) {
						if (n.output[j].name === plug.output.name) {
							outputs.push(n.output[j]);
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
			let n = nodeList[i];
			for (let k = 0; k < n.input.length; ++k) {
				let input = n.input[k];
				if (varnameToInput.hasOwnProperty(input.nodeVarname)) {
					if (varnameToInput[input.nodeVarname].name === input.name) {
						inputs.push(input);
					}
				}
			}
			for (let k = 0; k < n.output.length; ++k) {
				let output = n.output[k];
				if (varnameToOutput.hasOwnProperty(output.nodeVarname)) {
					if (varnameToOutput[output.nodeVarname].name === output.name) {
						outputs.push(output);
					}
				}
			}
		}

		let group = {
			name : "Group",
			varname : "group_" + uuid(),
			nodes : nodeList,
			plugs : this.getPlugsFromNodes(nodeList),
			input : inputs,
			output : outputs
		};
		this.addGroup({ group : group });
		this.store.emit(Constants.MAKE_GROUP_CALLED, null);
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
			let data = payload.data;

			if (data.nodes && data.nodes.length > 0){
				this.store.data.nodes = [];
				for (let i in data.nodes){
					this.importNode({ nodeInfo : data.nodes[i]});
				}
			} else {
				console.log('import failed: nodes.length === 0');
			}
			if (data.plugs && data.plugs.length > 0){
				this.store.data.plugs = [];
				for (let i in data.plugs) {
					this.addPlug({ plugInfo : data.plugs[i]});
				}
			} else {
				console.log('import failed: plugs.length === 0');
			}
			for (let i in data) {
				if (data.hasOwnProperty(i)) {
					if (i !== "nodes" && i !== "plugs") {
						this.store.data[i] = data[i];
					}
				}
			}
			this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.getPlugs().length);
		}
	}

	/**
	 * ノードをペーストする.
	 */
	pasteNodes(payload) {
 		if (payload.hasOwnProperty('nodeInfoList')) {
			// ノード間のプラグリストのコピーを取得.
			let plugs = JSON.parse(JSON.stringify(this.getPlugsFromNodes(payload.nodeInfoList)));

			// ノードを追加
			for (let i = 0; i < payload.nodeInfoList.length; i = i + 1) {
				let src = payload.nodeInfoList[i];
				// varnameの変更のため既存のものを削除.
				let preVarname = src.varname;
				delete src.varname;
				// input, outputのvarnameのため既存のものを削除.
				for (let i = 0; i < src.input.length; i = i + 1) {
					delete src.input[i].nodeVarname;
				}
				for (let i = 0; i < src.output.length; i = i + 1) {
					delete src.output[i].nodeVarname;
				}
				this.addNode({ nodeInfo : src });
				let node = this.store.getNodes()[this.store.getNodes().length - 1];

				// プラグの接続を新規に作ったノードに変更.
				for (let k = 0; k < plugs.length; k = k + 1) {
					if (plugs[k].input.nodeVarname === preVarname) {
						plugs[k].input.nodeVarname = node.varname;
					}
					if (plugs[k].output.nodeVarname === preVarname) {
						plugs[k].output.nodeVarname = node.varname;
					}
				}
			}

			// プラグを追加
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
					this.store.emit(Constants.NODE_SELECTE_CHANGED, null, n.node, n.index);
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
							this.store.emit(Constants.NODE_SELECTE_CHANGED, null, n.node, n.index);
						}
					}
				}
			} else {
				for (let i = 0; i < this.store.getNodes().length; i = i + 1) {
					// if (this.store.getNodes()[i].select) {
						this.store.getNodes()[i].select = false;
						this.store.emit(Constants.NODE_SELECTE_CHANGED, null, this.store.getNodes()[i], i);
					// }
				}
			}
		}
	}

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
					// 既に繋がっている入力端子に繋げようとした
					return;
				}
			}
			if (payload.plugInfo.input.nodeVarname === payload.plugInfo.output.nodeVarname) {
				// 同じノードに繋げようとした
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
}

ActionExecuter.initialData = {
	"varname": "invalid",
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
