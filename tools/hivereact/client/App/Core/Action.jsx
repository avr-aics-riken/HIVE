import Dispatcher from "./Dispatcher.jsx";

export default class HiveAction {
	constructor(id) {
		this.dispatcher = Dispatcher;
		this.id = id;
	}

	/**
	 * ノードを追加する
	 * @param nodeInfo ノード情報
	 */
	addNode(nodeInfo) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "addNode",
			nodeInfo : nodeInfo
		});
	}

	/**
	 * ノードを削除する
	 * @param varname ノード変数名
	 */
	deleteNode(varname) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "deleteNode",
			varname : varname
		});
	}

	/**
	 * ノードを変更する
	 */
	changeNode(nodeInfo) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "changeNode",
			nodeInfo : nodeInfo
		});
	}

}
