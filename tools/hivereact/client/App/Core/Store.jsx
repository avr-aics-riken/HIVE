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

        // [s] コンポーネント
        this.components = [];

		this.addNode = this.addNode.bind(this);
		this.deleteNode = this.deleteNode.bind(this);
        this.actionHandler = this.actionHandler.bind(this);
        this.addComponent = this.addComponent.bind(this);
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

    getComponents(){
        return this.components;
    }

	/**
	 * 全てのノードリストを返す
	 */
	getNodes() {
		return this.nodes;
	}

	/**
	 * アクションハンドラ
	 * @private
	 */
	actionHandler(payload) {
        // [s]
        this.addComponent(payload.data);

		if (payload && this.hasOwnProperty(payload.actionType)) {
			if (payload.hasOwnProperty("id") && payload.id === this.dispatchToken) {
				(() => {
					this[payload.actionType].bind(this)(payload);
				}());
			}
		}
	}

	changeNode() {
		console.log("change node at Core.Store");
	}

	/**
	 * ノード追加
	 */
	addNode(payload) {
		if (payload.hasOwnProperty('nodeInfo')) {
			this.nodes.push(payload.nodeInfo);
			this.emit(Store.NODE_COUNT_CHANGED, null, this.nodes.length);
		}
	}

	/**
	 * ノード削除
	 */
	 deleteNode(payload) {
 		if (payload.hasOwnProperty('varname')) {
 			for (let i = 0; i < this.nodes.length; i = i + 1) {
				if (this.nodes[i].varname === payload.varname) {
					this.nodes.splice(i, 1);
		 			this.emit(Store.NODE_COUNT_CHANGED, null, this.nodes.length);
					break;
				}
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
					this.emit(Store.NODE_CHANGED, null, this.nodes[i], i);
				}
			}
 		}
 	}

	/**
     * [s]
     * コンポーネントの追加
	 */
    addComponent(data) {
        this.components.push(data);
        this.emit(Store.ADD_COMPONENT);
    }
}
Store.NODE_CHANGED = "node_changed";
Store.NODE_COUNT_CHANGED = "node_count_changed";
Store.IMAGE_RECIEVED = "image_revieved";
Store.ADD_COMPONENT = "add_component";
