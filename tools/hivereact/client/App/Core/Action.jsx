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

	/**
	 * ノードを複数変更する
	 */
	changeNodes(nodeInfoList) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "changeNodes",
			nodeInfoList : nodeInfoList
		});
	}

	/**
	 * プラグを追加する
	 * @param plugInfo プラグ情報
	 */
	addPlug(plugInfo) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "addPlug",
			plugInfo : plugInfo
		});
	}

	/**
	 * プラグを削除する
	 * @param plugInfo プラグ情報
	 */
	deletePlug(plugInfo) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "deletePlug",
			plugInfo : plugInfo
		});
	}

	/**
	 * パネルを非表示にする
	 * @param varname ノード変数名
	 */
	hiddenPanel(varname) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "hiddenPanel",
			varname : varname
		});
	}

	/**
	 * ノードを選択する
	 */
	selectNode(nodeVarnameList) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "selectNode",
			nodeVarnameList : nodeVarnameList
		});
	}

	/**
	 * ノードの選択を解除する
	 * @parma nodeVarnameList 対象のノードvarnameリスト。全てのノードを対象にする場合は空リストを入れる.
	 * @param excludeVarname 対象外ノード
	 */
	unSelectNode(nodeVarnameList, excludeVarname) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "unSelectNode",
			nodeVarnameList : nodeVarnameList,
			excludeVarname : excludeVarname
		});
	}
}
