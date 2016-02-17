import EventEmitter from 'eventemitter3'
import Dispatcher from "./HiveDispatcher.jsx"
import HiveCore from "./HiveCore.jsx"

export default class HiveStore extends EventEmitter {
	constructor() {
		super();
		this.image = null;
		this.dispatchToken = Dispatcher.register(() => {
			console.log("action registered");
			return this.actionHandler;
		});
	}

	initEmitter(core) {
		core.on(HiveCore.NODE_CHANGED, (err, data) => {
			if (!err) {
				this.changeNode(data);
			} else {
				console.error(err);
			}
		});
		core.on(HiveCore.IMAGE_RECIEVED, (err, param, data) => {
			this.emit(HiveStore.IMAGE_RECIEVED, err, param, data);
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
		console.log("change node at HiveStore");
	}
}
HiveStore.NODE_CHANGED = "node_changed";
HiveStore.IMAGE_RECIEVED = "image_revieved";
