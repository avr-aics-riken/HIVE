import EventEmitter from 'eventemitter3'
import Dispatcher from "./Dispatcher.jsx"
import Hive from "../HIVE"

export default class Store extends EventEmitter {
	constructor() {
		super();
		this.image = null;
		this.dispatchToken = Dispatcher.register(() => {
			console.log("action registered");
			return this.actionHandler;
		});
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
	 * アクションハンドラ
	 * @private
	 */
	actionHandler(payload) {
		console.log("actionHandler");
		if (payload && this.hasOwnProperty(payload.actionType)) {
			if (payload.id && payload.id === this.dispatchToken) {
				() => {
					this[payload.actionType].bind(this)(payload);
				}
			}
		}
	}

	changeNode() {
		console.log("change node at Core.Store");
	}
}
Store.NODE_CHANGED = "node_changed";
Store.IMAGE_RECIEVED = "image_revieved";
