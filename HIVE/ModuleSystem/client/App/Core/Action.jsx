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
	 * HIVEを再起動する
	 */
	rebootHive() {
        this.dispatcher.dispatch({
			id :this.id,
			actionType: "rebootHive"
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
		if (varname) {
			this.addNode({name : nodeName, varname : varname});
		} else {
			this.addNode({name : nodeName});
		}
	}

    /**
	 * SceneScriptをエクスポートする
	 * @param varname ノード変数名
	 */
	exportSceneScript(varname) {
        if (!varname) {
            varname = '';
        }
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "exportSceneScript",
			varname : varname
		});
	}

    /**
	 * 現在のグループをエクスポートする
	 * @param varname ノード変数名
	 */
	exportGroupNode(varname) {
        if (!varname) {
            varname = '';
        }
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "exportGroupNode",
			varname : varname
		});
	}

    /**
	 * グループをインポートする
	 * @param varname ノード変数名
	 */
	 /* loadでできるようになった
	importGroupNode(data) {
        this.dispatcher.dispatch({
			id :this.id,
			actionType: "importGroupNode",
            data: data
		});
	}
	*/

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
	 * ノードを整列する
	 */
	align() {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "align"
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
	 * 選択中のノードのグループを解除する
	 */
	unGroup() {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "unGroup"
		});
	}

	/**
	 * グループを変更(移動)する
	 */
	changeGroup(groupVarname) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "changeGroup",
			groupVarname : groupVarname
		});
	}

	/**
	 * ファイルに保存する
	 */
	save() {
		this.dispatcher.dispatch({
			id : this.id,
			actionType : "save"
		});
	}

	/**
	 * データから読み込む
	 * @param data 読み込むデータ.
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
	 * ノードを整列させる
	 * @param zoom ズーム値
	 * @param rect ビューポートのみためのleft,top,right,bottom
	 * @param realRect 実際のrect
	 * @param nodeSizes {nodevarname : [width, height], ..} 形式のノードサイズマップ
	 */
	alignNodes(zoom, rect, realRect, nodeSizes) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "alignNodes",
			zoom : zoom,
			rect :rect,
			realRect : realRect,
			nodeSizes : nodeSizes
		});
	};

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

	/**
	 * フレーム番号を変更する.
	 * @parma keyFrame フレーム番号.
	 */
	changeFrame(frame) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "changeFrame",
			frame : frame
		});
	}

	/**
	 * キーフレームを追加する
	 * @param frame フレーム番号
	 * @param node ノード
	 * @param input キーフレームを登録する入力
	 */
	addKeyFrame(frame, node, input) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "addKeyFrame",
			frame : frame,
			node : node,
			input : input
		});
	}

	/**
	 * キーフレームを削除する
	 * @param frame フレーム番号
	 * @param node ノード
	 * @param input キーフレームを登録する入力
	 */
	deleteKeyFrame(frame, node, input) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "deleteKeyFrame",
			frame : frame,
			node : node,
			input : input
		});
	}

	/**
	 * ファイルブラウザを開く.
	 * @param key 任意のキー. OK_FILE_BROWSERイベントで返される
	 */
	openFilebrowser(key) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "openFilebrowser",
			key : key
		});
	}

	/**
	 * ファイルブラウザでOKされた通知を送る.
	 * @param key 任意のキー. openFilebrowserの引数のキー
	 * @param value 選択されたパス
	 */
	okFileBrowser(key, value) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "okFileBrowser",
			key : key,
			value : value
		});
	}

	/**
	 * ラベルダイアログを開く.
	 * @param key 任意のキー. OK_LABEL_DIALOGイベントで返される
	 */
	openLabelDialog(key, callback) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "openLabelDialog",
			key : key,
			callback : callback
		});
	}

	/**
	 * ラベルダイアログがOKされた通知を送る.
	 * @param key 任意のキー. openLabelDialogの引数のキー
	 * @param value 選択されたパス
	 */
	okLabelDialog(key, value) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "okLabelDialog",
			key : key,
			value : value
		});
	}
	
	/**
	 * Messageダイアログを開く.
	 * @param key 任意のキー. OK_MESSAGE_DIALOGイベントで返される
	 */
	openMessageDialog(key, callback) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "openMessageDialog",
			key : key,
			callback : callback
		});
	}
	
	/**
	 * MessageダイアログがOKされた通知を送る.
	 * @param key 任意のキー. openMessageDialogの引数のキー
	 * @param value 選択されたパス
	 */
	okMessageDialog(key, value) {
		this.dispatcher.dispatch({
			id :this.id,
			actionType: "okMessageDialog",
			key : key,
			value : value
		});
	}
}
