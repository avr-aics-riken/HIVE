
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
}
