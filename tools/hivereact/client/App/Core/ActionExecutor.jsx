import Constants from "./Constants.jsx"

export default class ActionExecuter {
	constructor(store) {
        
        //
        // !!! don't have any data. !!!
        //
        
        // only Functions		
		this.addNode = this.addNode.bind(store);
		this.deleteNode = this.deleteNode.bind(store);
		this.changeNode = this.changeNode.bind(store);
		this.changeNodes = this.changeNodes.bind(store);
		this.addPlug = this.addPlug.bind(store);
		this.deletePlug = this.deletePlug.bind(store);
		this.hiddenPanel = this.hiddenPanel.bind(store);
		this.selectNode = this.selectNode.bind(store);
		this.unSelectNode = this.unSelectNode.bind(store);
	}

	/**
	 * ノード追加
	 */
	addNode(payload) {
		console.log("addnode")
		if (payload.hasOwnProperty('nodeInfo')) {
			let node = null;
			if (payload.nodeInfo.hasOwnProperty('name')) {
				node = this.nodeSystem.CreateNodeInstance(payload.nodeInfo.name);
			}
			if (payload.nodeInfo.hasOwnProperty('panel')) {
				if (payload.nodeInfo.panel.zindex === 0) {
					payload.nodeInfo.panel.zindex = this.data.nodes.length + 1;
				}
			}
			if (node) {
				this.data.nodes.push(node);
			}
			this.emit(Constants.NODE_COUNT_CHANGED, null, this.data.nodes.length);
			this.emit(Constants.NODE_ADDED, null, payload.nodeInfo);
		}
	}

	/**
	 * ノード削除
	 */
	deleteNode(payload) {
		if (payload.hasOwnProperty('varname')) {
			let n = this.getNode(payload.varname);
			if (n) {
				this.data.nodes.splice(n.index, 1);
				this.emit(Constants.NODE_COUNT_CHANGED, null, this.nodes.length);
				this.emit(Constants.NODE_DELETED, null, n.node);

				// 関連するプラグを削除.
				for (let i = 0; i < this.plugs.length; i = i + 1) {
					let plug = this.plugs[i];
					if (plug.input.nodeVarname === payload.varname) {
						this.deletePlug({ plugInfo : plug });
					} else if (plug.output.nodeVarname === payload.varname) {
						this.deletePlug({ plugInfo : plug });
					}
				}
			}
		}
	}

	/**
 	 * ノード変更.
 	 */
 	changeNode(payload) {
 		if (payload.hasOwnProperty('nodeInfo')) {
 			for (let i = 0; i < this.data.nodes.length; i = i + 1) {
				if (this.data.nodes[i].varname === payload.nodeInfo.varname) {
					let preInputs = JSON.stringify(this.data.nodes[i].input);
					let postInputs = JSON.stringify(payload.nodeInfo.input);
					let preSelect = this.data.nodes[i].select;
					let postSelect = payload.nodeInfo.select;
					let uiComponent = this.data.nodes[i].uiComponent;
					this.data.nodes[i] = JSON.parse(JSON.stringify(payload.nodeInfo));
					this.data.nodes[i].uiComponent = uiComponent;
					this.emit(Constants.NODE_CHANGED, null, this.data.nodes[i], i);
					if (preInputs !== postInputs) {
						this.emit(Constants.NODE_INPUT_CHANGED, null, this.data.nodes[i], i);
					}
					if (preSelect !== postSelect) {
						this.emit(Constants.NODE_SELECTE_CHANGED, null, this.data.nodes[i], i);
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
	 * ノードを選択する
	 */
	selectNode(payload) {
		if (payload.hasOwnProperty('nodeVarnameList')) {
			for (let i = 0; i < payload.nodeVarnameList.length; i = i + 1) {
				let n = this.getNode(payload.nodeVarnameList[i]);
				if (n && !n.nodeselect) {
					n.node.select = true;
					this.emit(Constants.NODE_SELECTE_CHANGED, null, n.node, n.index);
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
					let n = this.getNode(payload.nodeVarnameList[i]);
					if (n.node.select) {
						if (!payload.hasOwnProperty('excludeVarname') || payload.excludeVarname !== n.nodevarname) {
							n.node.select = false;
							this.emit(Constants.NODE_SELECTE_CHANGED, null, n.node, n.index);
						}
					}
				}
			} else {
				for (let i = 0; i < this.data.nodes.length; i = i + 1) {
					// if (this.data.nodes[i].select) {
						this.data.nodes[i].select = false;
						this.emit(Constants.NODE_SELECTE_CHANGED, null, this.data.nodes[i], i);
					// }
				}
			}
		}
	}

	/**
	 * プラグを追加
	 */
	addPlug(payload) {
		if (payload.hasOwnProperty('plugInfo')) {
			for (let i = 0; i < this.plugs.length; i = i + 1) {
				if (this.data.plugs[i].output.nodeVarname === payload.plugInfo.output.nodeVarname &&
					this.data.plugs[i].output.name === payload.plugInfo.output.name &&
					this.data.plugs[i].input.nodeVarname === payload.plugInfo.input.nodeVarname &&
					this.data.plugs[i].input.name === payload.plugInfo.input.name) {
					// 同じプラグが既にあった.
					return;
				}
			}
			this.data.plugs.push(payload.plugInfo);
			this.emit(Constants.PLUG_COUNT_CHANGED, null, this.data.plugs.length);
		}
	}

	/**
	 * プラグ削除
	 */
	deletePlug(payload) {
		if (payload.hasOwnProperty('plugInfo')) {
			for (let i = 0; i < this.plugs.length; i = i + 1) {
				if (this.data.plugs[i].output.nodeVarname === payload.plugInfo.output.nodeVarname &&
					this.data.plugs[i].input.nodeVarname === payload.plugInfo.input.nodeVarname) {
					let plug = this.plugs[i];
					this.data.plugs.splice(i, 1);
					this.emit(Constants.PLUG_COUNT_CHANGED, null, this.data.plugs.length);
					this.emit(Constants.PLUG_DELETED, null, plug);
					break;
				}
			}
		}
	}

	/**
	 * パネル非表示
	 */
	hiddenPanel(payload) {
		if (payload.hasOwnProperty('varname')) {
			let n = this.getNode(payload.varname);
			if (n) {
				let node = n.node;
				let index = n.index;
				// node.panel.visible = false;
				node.panel.visible = !node.panel.visible; // temp
				this.emit(Constants.NODE_CHANGED, null, node, index);
			}
        }
    }
}
