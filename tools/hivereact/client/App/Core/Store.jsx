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

		this.addNode = this.addNode.bind(this);
		this.deleteNode = this.deleteNode.bind(this);
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
}
Store.NODE_CHANGED = "node_changed";
Store.NODE_COUNT_CHANGED = "node_count_changed";
Store.IMAGE_RECIEVED = "image_revieved";
