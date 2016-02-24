
export default class Action {
	constructor(dispatcher, id) {
		this.dispatcher = dispatcher;
		this.id = id;
	}

	/**
	 * プラグ位置を変更する.
	 * @param nodeVarname プラグの端子が接続されているノードのvarname
	 * @param isInput 端子が入力端子かどうか。出力端子の場合はfalseを入れる.
	 * @param name プラグの端子名
	 * @param pos 位置。[x, y]
	 */
	changePlugPosition(nodeVarname, isInput, name, pos) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "changePlugPosition",
			nodeVarname : nodeVarname,
			isInput : isInput,
			name : name,
			pos : pos
		});
	}

	/**
	 * ノードを選択する
	 */
	selectNode(node) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "selectNode",
			node : node
		});
	}

	/**
	 * ノードの選択を解除する
	 * @parma node 対象のノード。全てのノードを対象にする場合はnullを入れる.
	 * @param excludeNode 対象外ノード
	 */
	unSelectNode(node, excludeNode) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "unSelectNode",
			node : node,
			excludeNode : excludeNode
		});
	}

	/**
	 * ノードを移動させる
	 * @param mv 移動量 { x : x移動量, y : y移動量 }
	 */
	moveNode(mv) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "moveNode",
			mv : mv
		});
	}
}
