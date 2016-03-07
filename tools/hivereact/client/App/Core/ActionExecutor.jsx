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
		this.addNode = this.addNode.bind(this);
		this.deleteNode = this.deleteNode.bind(this);
		this.deleteNodes = this.deleteNodes.bind(this);
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
		this.copy = this.copy.bind(this);
		this.paste = this.paste.bind(this);
		this.delete = this.delete.bind(this);
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

	assignInitialNodeValue(node) {
		for (let key in ActionExecuter.initialData) {
			if (!node.hasOwnProperty(key)) {
				node[key] = JSON.parse(JSON.stringify(ActionExecuter.initialData[key]));
			}
		}
		if (node.uiFunc === "") {
			// UI無し
			delete node.panel.visible;
		}
	}
	/**
	 * ノード追加
	 */
	addNode(payload) {
		if (payload.hasOwnProperty('nodeInfo')) {
			let node = null;
			if (payload.nodeInfo.hasOwnProperty('name')) {
				node = this.store.nodeSystem.CreateNodeInstance(payload.nodeInfo.name);
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
					payload.nodeInfo.panel.zindex = this.store.data.nodes.length + 1;
				}
			}
			if (payload.nodeInfo.hasOwnProperty('input')) {
				node.input = payload.nodeInfo.input;
			}
			if (node) {
				this.assignInitialNodeValue(node);
				var nl = this.store.getNodes();
				positionGetter(node, nl);
				this.store.data.nodes.push(node);
			}
			this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.data.nodes.length);
			this.store.emit(Constants.NODE_ADDED, null, node);
		}
	}

	/**
	 * ノード削除
	 */
	deleteNode(payload) {
		if (payload.hasOwnProperty('varname')) {
			let n = this.store.getNode(payload.varname);
			if (n) {
				this.store.data.nodes.splice(n.index, 1);

				// 関連するプラグを削除.
				for (let i = this.store.data.plugs.length - 1; i >= 0; i = i - 1) {
					let plug = this.store.data.plugs[i];
					if (plug.input.nodeVarname === payload.varname) {
						this.deletePlug({ plugInfo : plug });
					} else if (plug.output.nodeVarname === payload.varname) {
						this.deletePlug({ plugInfo : plug });
					}
				}
				this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.data.nodes.length);
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
 	 * ノード変更.
 	 */
 	changeNode(payload) {
 		if (payload.hasOwnProperty('nodeInfo')) {
 			for (let i = 0; i < this.store.data.nodes.length; i = i + 1) {
				if (this.store.data.nodes[i].varname === payload.nodeInfo.varname) {
					let dstNode = this.store.data.nodes[i];
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
						if (info !== "uiComponent" && this.store.data.nodes[i].hasOwnProperty(info)) {
							this.store.data.nodes[i][info] = JSON.parse(JSON.stringify(payload.nodeInfo[info]));
						}
					}

					this.store.emit(Constants.NODE_CHANGED, null, this.store.data.nodes[i], i);
					if (hasInput && preInputs !== postInputs) {
						this.store.emit(Constants.NODE_INPUT_CHANGED, null, this.store.data.nodes[i], i);
					}
					if (hasSelect && preSelect !== postSelect) {
						this.store.emit(Constants.NODE_SELECTE_CHANGED, null, this.store.data.nodes[i], i);
					}
					if (hasPanel && prePanel !== postPanel) {
						this.store.emit(Constants.PANEL_CHANGED, null, this.store.data.nodes[i], i);
					}
					if (hasPanel && prePanelVisible !== postPanelVisible) {
						this.store.emit(Constants.PANEL_VISIBLE_CHANGED, null, this.store.data.nodes[i], i);
					}
					if (hasPanel && prePanelSize !== postPanelSize) {
						this.store.emit(Constants.PANEL_SIZE_CHANGED, null, this.store.data.nodes[i], i);
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

		for (let i = 0; i < this.store.data.plugs.length; i = i + 1) {
			let plug = this.store.data.plugs[i];
			if (varnameToNodes.hasOwnProperty(plug.input.nodeVarname) &&
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
	 * ノードをペーストする.
	 */
	pasteNodes(payload) {
 		if (payload.hasOwnProperty('nodeInfoList')) {
			// ノード間のプラグリストのコピーを取得.
			let plugs = JSON.parse(JSON.stringify(this.getPlugsFromNodes(payload.nodeInfoList)));

			// ノードを追加
			for (let i = 0; i < payload.nodeInfoList.length; i = i + 1) {
				let src = payload.nodeInfoList[i];
				let preVarname = src.varname;
				delete src.varname;
				this.addNode({ nodeInfo : src });
				let node = this.store.data.nodes[this.store.data.nodes.length - 1];

				// プラグの接続を新規に作ったノードに変更.
				for (let k = 0; k < plugs.length; k = k + 1) {
					console.log(plugs[k].input.nodeVarname, preVarname);
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
				for (let i = 0; i < this.store.data.nodes.length; i = i + 1) {
					// if (this.store.data.nodes[i].select) {
						this.store.data.nodes[i].select = false;
						this.store.emit(Constants.NODE_SELECTE_CHANGED, null, this.store.data.nodes[i], i);
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
            this.store.data.nodes.push(payload.nodeInfo);
			this.store.emit(Constants.NODE_COUNT_CHANGED, null, this.store.data.nodes.length);
			this.store.emit(Constants.NODE_ADDED, null, payload.nodeInfo);
		}
	}

	/**
	 * プラグを追加
	 */
	addPlug(payload) {
		if (payload.hasOwnProperty('plugInfo')) {
			for (let i = 0; i < this.store.data.plugs.length; i = i + 1) {
				if (this.store.data.plugs[i].input.nodeVarname === payload.plugInfo.input.nodeVarname &&
					this.store.data.plugs[i].input.name === payload.plugInfo.input.name) {
					// 既に繋がっている入力端子に繋げようとした
					return;
				}
			}
			this.store.data.plugs.push(payload.plugInfo);
			this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.data.plugs.length);
			this.store.emit(Constants.PLUG_ADDED, null, payload.plugInfo);
		}
	}

	/**
	 * プラグ削除
	 */
	deletePlug(payload) {
		if (payload.hasOwnProperty('plugInfo')) {
			let info = payload.plugInfo;
			for (let i = 0; i < this.store.data.plugs.length; i = i + 1) {
				let input = this.store.data.plugs[i].input;
				let output = this.store.data.plugs[i].output;
				if (output.nodeVarname === info.output.nodeVarname &&
					input.nodeVarname === info.input.nodeVarname &&
					output.name === info.output.name) {

					if (Array.isArray(input.array)) {
						for (let k = 0; k < input.array.length; k = k + 1) {
							if (input.array[k].name === info.input.name) {
								let plug = this.store.data.plugs[i];
								this.store.data.plugs.splice(i, 1);
								this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.data.plugs.length);
								this.store.emit(Constants.PLUG_DELETED, null, plug);
								return;
							}
						}
					} else if (input.name === info.input.name) {
						let plug = this.store.data.plugs[i];
						this.store.data.plugs.splice(i, 1);
						this.store.emit(Constants.PLUG_COUNT_CHANGED, null, this.store.data.plugs.length);
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
