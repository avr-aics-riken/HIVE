import EventEmitter from 'eventemitter3'
import { Dispatcher } from "../../Core"

export default class Store extends EventEmitter {
	constructor() {
		super();
		this.dispatchToken = Dispatcher.register(this.actionHandler.bind(this));

		this.plugPositions = [];
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
		if (payload && this.hasOwnProperty(payload.actionType)) {
			if (payload.hasOwnProperty("id") && payload.id === this.dispatchToken) {
				(() => {
					this[payload.actionType].bind(this)(payload);
				}());
			}
		}
	}

	/**
	 * プラグ位置を変更する.
	 * @private
	 */
	changePlugPosition(payload) {
		console.log("change plug position", payload);
	}
}
