
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

	/**
	 * プラグのドラッグを開始する
	 * @param id 端子id
	 * @param inputPos 入力端子側のプラグ位置
	 * @param outputPos 出力端子側のプラグ位置
	 */
	dragPlug(plugInfo, inputPos, outputPos) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "dragPlug",
			plugInfo : plugInfo,
			inputPos : inputPos,
			outputPos : outputPos
		});
	}

	/**
	 * プラグのドラッグを終了する
	 */
	endDragPlug(plugInfo, inputPos, outputPos) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "endDragPlug",
			plugInfo : plugInfo,
			inputPos : inputPos,
			outputPos : outputPos
		});
	}

	/**
	 * プラグ端子を選択する
	 */
	selectPlugHole(plugInfo) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "selectPlugHole",
			plugInfo : plugInfo
		});
	}

	/**
	 * プラグ端子の選択を解除する
	 */
	unSelectPlugHoles() {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "unSelectPlugHoles"
		});
	}

	/**
	 * プラグ端子の接続を解除
	 */
	disconnectPlugHole(plugInfo) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "disconnectPlugHole",
			plugInfo : plugInfo
		});
	}

}
