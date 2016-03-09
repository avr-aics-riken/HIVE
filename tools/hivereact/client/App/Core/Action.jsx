import Dispatcher from "./Dispatcher.jsx";

export default class Action {
	constructor(id) {
		this.dispatcher = Dispatcher;
		this.id = id;
	}

    /**
	 * Console出力画面を表示する
	 * @param show 表示情報
	 */
	showConsoleOutput(show) {
        this.dispatcher.dispatch({
			id :this.id,
			actionType: "showConsoleOutput",
			show : show
		});
    }

    /**
	 * Layoutを変更する
	 * @param mode レイアウト名称
	 */
    setLayout(mode) {
        this.dispatcher.dispatch({
			id :this.id,
			actionType: "setLayout",
			mode : mode
		});
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
	 * ノードを追加する
	 * @param nodeInfo ノード情報
	 * @param varname 特定のvarnameで作成する場合文字列を入れる。通常はnullを入れる。
	 */
	addNodeByName(nodeName, varname) {
/*
        let node = this.nodeSystem.CreateNodeInstance(nodeName);
        if (!node) {
            return false;
        }

		// create unique varname
		node.varname += name + uuid();
        for (let i = 0; true; i = i + 1) {
			let foundSameName = false;
			let name = node.varname + "_" + String(i);
			for (let i = 0; i < this.state.nodes.length; i = i + 1) {
				if (this.state.nodes[i].varname === name) {
					foundSameName = true;
					break;
				}
			}
			if (!foundSameName) {
				node.varname = name;
				break;
			}
		}

		node.pos = [ 200, 200 ];

        // insert position
        let x, y;
        x = node.panel.pos[0];
        y = node.panel.pos[1];
        for (let i in nodes) {
            let panel = nodes[i].panel;
            while (true) {
                let f = true;
                if (Math.abs(x - panel.pos[0]) < 50) {
                    x += 50; f = false;
                }
                if (Math.abs(y - panel.pos[1]) < 50) {
                    y += 50; f = false;
                }
                if (f) {
                    break;
                }
            }
        }
        node.panel.pos = [x, y];
*/
		if (varname) {
			this.addNode({name : nodeName, varname : varname});
		} else {
			this.addNode({name : nodeName});
		}
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
	 * ノードを削除する
	 * @param varname ノード変数名リスト
	 */
	deleteNodes(varnameList) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "deleteNodes",
			varnameList : varnameList
		});
	}

	/**
	 * 現在のノード階層に対して入力を追加する.
	 */
	publishInput(inputData) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "publishInput",
			data : inputData
		});
	}

	/**
	 * 現在のノード階層に対して入力を削除する.
	 */
	unPublishInput(inputData) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "unPublishInput",
			data : inputData
		});
	}

	/**
	 * 現在のノード階層に対して出力を追加する.
	 */
	publishOutput(outputData) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "publishOutput",
			data : outputData
		});
	}

	/**
	 * 現在のノード階層に対して出力を削除する.
	 */
	unPublishOutput(outputData) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "unPublishOutput",
			data : outputData
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
	 * ノードの入力を変更する
	 * @param inputInfo 以下の形式でノード入力を指定する
	 * {
	 *    varname : varname,
	 *    input : {
	 *         ノード入力名1: ノード入力の値1,
	 *         ノード入力名2: ノード入力の値2, ..
	 *    }
	 * }
	 */
	changeNodeInput(inputInfo) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "changeNodeInput",
			inputInfo : inputInfo
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
	 * コピーする.
	 */
	copy() {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "copy"
		});
	}

	/**
	 * ペーストする.
	 */
	paste() {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "paste"
		});
	}

	/**
	 * 削除する
	 */
	delete() {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "delete"
		});
	}

	/**
	 * すべてのデータをクリア(削除）する
	 */
	clearAll() {
		this.dispatcher.dispatch({
			id : this.id,
			actionType : "clearAll"
		});
	}

	/**
	 * グループを作成する
	 */
	makeGroup() {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "makeGroup"
		});
	}

	/**
	 * グループを移動する
	 */
	digGroup(groupVarname) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "digGroup",
			groupVarname : groupVarname
		});
	}

	/**
	 * 保存する
	 */
	save() {
		this.dispatcher.dispatch({
			id : this.id,
			actionType : "save"
		});
	}

	/**
	 * 読み込む
	 */
	load(data) {
		this.dispatcher.dispatch({
			id : this.id,
			actionType : "load",
			data : data
		});
	}

	/**
	 * ノードをペーストする.
	 */
	pasteNodes(nodeInfoList) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "pasteNodes",
			nodeInfoList : nodeInfoList
		});
	}

	/**
	 * ノードをインポートする
	 * @param nodeInfo ノード情報
	 */
	importNode(nodeInfo) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "importNode",
			nodeInfo : nodeInfo
		});
	}

	/**
	 * プラグを追加する
	 * @param plugInfo プラグ情報
	 * プラグ情報は以下の形式
	 * {
	 *    output : {
	 *        nodeVarname : ノード変数名
	 *        name : プラグ名
	 *    }
	 *    input : {
	 *        nodeVarname : ノード変数名
	 *        name : プラグ名
	 *    }
	 * }
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
	 * パネルの表示状態を切り替える
	 * @param varname ノード変数名
	 * @param isVisible
	 */
	changePanelVisible(varname, isVisible) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "changePanelVisible",
			varname : varname,
			visible : isVisible
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
