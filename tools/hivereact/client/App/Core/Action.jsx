import Dispatcher from "./Dispatcher.jsx";

function uuid() {
    var uuid = "", i, random;
    for (i = 0; i < 32; i++) {
        random = Math.random() * 16 | 0;

        if (i == 8 || i == 12 || i == 16 || i == 20) {
            uuid += "_"
        }
        uuid += (i == 12 ? 4 : (i == 16 ? (random & 3 | 8) : random)).toString(16);
    }
    return uuid;
}

export default class Action {
	constructor(id, nodeSystem) {
		this.dispatcher = Dispatcher;
		this.id = id;
        this.nodeSystem = nodeSystem;
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
	 */
	addNodeByName(nodeName) {
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
		this.addNode({name : nodeName});
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
