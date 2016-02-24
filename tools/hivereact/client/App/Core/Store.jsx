import EventEmitter from 'eventemitter3'
import Dispatcher from "./Dispatcher.jsx"
import Hive from "../HIVE"

export default class Store extends EventEmitter {
	constructor() {
		super();
		this.image = null;

		this.dispatchToken = Dispatcher.register(this.actionHandler.bind(this));

		// 全てのノード
		this.nodes = [];

		// 全てのプラグ
		this.plugs = [];

		this.addNode = this.addNode.bind(this);
		this.getNode = this.getNode.bind(this);
		this.deleteNode = this.deleteNode.bind(this);
		this.changeNode = this.changeNode.bind(this);
		this.changeNodes = this.changeNodes.bind(this);
		this.actionHandler = this.actionHandler.bind(this);
		this.addPlug = this.addPlug.bind(this);
		this.deletePlug = this.deletePlug.bind(this);
        this.hiddenPanel = this.hiddenPanel.bind(this);
		this.getNode = this.getNode.bind(this);
		this.selectNode = this.selectNode.bind(this);
		this.unSelectNode = this.unSelectNode.bind(this);
	}

	initEmitter(core) {
		core.on(Hive.NODE_CHANGED, (err, data) => {
			if (!err) {
				this.changeNode(data);
			} else {
				console.error(err);
			}
		});
		core.on(Hive.IMAGE_RECIEVED, (err, param, data) => {
			this.emit(Store.IMAGE_RECIEVED, err, param, data);
		});
	}

	/**
	 * dispatchTokenを返す.
	 */
	getDispatchToken() {
		return this.dispatchToken;
	}

	/**
	 * 全てのノードリストを返す
	 */
	getNodes() {
		return this.nodes;
	}

	/**
	 * 全てのプラグリストを返す
	 */
	getPlugs() {
		return this.plugs;
	}

	/**
	 * 特定のnodeとそのindexを返す.
	 */
	getNode(varname) {
		for (let i = 0; i < this.nodes.length; i = i + 1) {
			if (this.nodes[i].varname === varname) {
				return { node : this.nodes[i], index : i }
			}
		}
		return null;
	}

	/**
	 * アクションハンドラ
	 * @private
	 */
	actionHandler(payload) {
		if (payload && this.hasOwnProperty(payload.actionType)) {
			if (payload.hasOwnProperty("id") && payload.id === this.dispatchToken) {
				(() => {
					this[payload.actionType].bind(this)(payload);
				}());
			}
		}
	}

	/**
	 * ノード追加
	 */
	addNode(payload) {
		if (payload.hasOwnProperty('nodeInfo')) {
            if(payload.nodeInfo.panel.zindex === 0){
                payload.nodeInfo.panel.zindex = this.nodes.length + 1;
            }
			this.nodes.push(payload.nodeInfo);
			this.emit(Store.NODE_COUNT_CHANGED, null, this.nodes.length);
			this.emit(Store.NODE_ADDED, null, payload.nodeInfo);
		}
	}

	/**
	 * ノード削除
	 */
	deleteNode(payload) {
		if (payload.hasOwnProperty('varname')) {
			let n = this.getNode(payload.varname);
			if (n) {
				this.nodes.splice(n.index, 1);
				this.emit(Store.NODE_COUNT_CHANGED, null, this.nodes.length);
				this.emit(Store.NODE_DELETED, null, n.node);
			}
		}
	}

	/**
 	 * ノード変更.
 	 */
 	changeNode(payload) {
 		if (payload.hasOwnProperty('nodeInfo')) {
 			for (let i = 0; i < this.nodes.length; i = i + 1) {
				if (this.nodes[i].varname === payload.nodeInfo.varname) {
					let preInputs = JSON.stringify(this.nodes[i].input);
					let postInputs = JSON.stringify(payload.nodeInfo.input);
					let preSelect = this.nodes[i].select;
					let postSelect = payload.nodeInfo.select;
					let uiComponent = this.nodes[i].uiComponent;
					this.nodes[i] = JSON.parse(JSON.stringify(payload.nodeInfo));
					this.nodes[i].uiComponent = uiComponent;
					this.emit(Store.NODE_CHANGED, null, this.nodes[i], i);
					if (preInputs !== postInputs) {
						this.emit(Store.NODE_INPUT_CHANGED, null, this.nodes[i], i);
					}
					if (preSelect !== postSelect) {
						this.emit(Store.NODE_SELECTE_CHANGED, null, this.nodes[i], i);
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
					this.emit(Store.NODE_SELECTE_CHANGED, null, n.node, n.index);
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
							this.emit(Store.NODE_SELECTE_CHANGED, null, n.node, n.index);
						}
					}
				}
			} else {
				for (let i = 0; i < this.nodes.length; i = i + 1) {
					// if (this.nodes[i].select) {
						this.nodes[i].select = false;
						this.emit(Store.NODE_SELECTE_CHANGED, null, this.nodes[i], i);
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
			this.plugs.push(payload.plugInfo);
			this.emit(Store.PLUG_COUNT_CHANGED, null, this.plugs.length);
		}
	}

	/**
	 * プラグ削除
	 */
	deletePlug(payload) {
		if (payload.hasOwnProperty('plugInfo')) {
			for (let i = 0; i < this.plugs.length; i = i + 1) {
				if (this.plugs[i].output.node === payload.output.node &&
					this.plugs[i].input.node === payload.input.node) {
					let plug = this.plugs[i];
					this.plugs.splice(i, 1);
					this.emit(Store.PLUG_COUNT_CHANGED, null, this.plugs.length);
					this.emit(Store.PLUG_DELETED, null, plug);
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
				this.emit(Store.NODE_CHANGED, null, node, index);
			}
        }
    }

}

Store.NODE_CHANGED = "node_changed";
Store.NODE_INPUT_CHANGED = "node_input_changed";
Store.PLUG_CHANGED = "plug_changed";
Store.NODE_COUNT_CHANGED = "node_count_changed";
Store.PLUG_COUNT_CHANGED = "plug_count_changed";
Store.IMAGE_RECIEVED = "image_revieved";
Store.NODE_ADDED = "node_added";
Store.NODE_DELETED = "node_deleted";
Store.NODE_SELECTE_CHANGED = "node_selected";
Store.PLUG_ADDED = "plug_added";
Store.PLUG_DELETED = "plug_deleted";
