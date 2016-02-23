
import { Dispatcher } from "../../Core"

export default class Action {
	constructor(id) {
		this.dispatcher = Dispatcher;
		this.id = id;
	}

	/**
	 * プラグ位置を変更する.
	 */
	changePlugPosition(plugID, inpos, outpos) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "changePlugPosition",
			plugID : plugID,
			inpos : inpos,
			outpos : outpos
		});
	}
}
