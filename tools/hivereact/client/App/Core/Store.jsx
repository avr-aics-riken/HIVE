import EventEmitter from 'eventemitter3'
import Dispatcher from "./Dispatcher.jsx"
import Hive from "../HIVE"
import ActionExecuter from "./ActionExecutor.jsx"
import NodeSystem from "../NodeSystem"
import Constants from "./Constants.jsx"

export default class Store extends EventEmitter {
    constructor() {
        super();

        // private:
        this.data = {
			name : "",    //シーン名
			varname : "root",
            nodes : [],   // 全てのノード
            plugs : [],   // 全てのプラグ
			input : [],   // シーンの入力端子
			output : [],  // シーンの出力端子
			nodePath : [] // 表示しているノード階層のパス
        }

		this.actionExecuter = new ActionExecuter(this);
        this.dispatchToken = Dispatcher.register(this.actionExecuter.actionHandler.bind(this.actionExecuter));

		this.initHive = this.initHive.bind(this);
		this.getNodes = this.getNodes.bind(this);
		this.getNode = this.getNode.bind(this);
		this.getNodeNameList = this.getNodeNameList.bind(this);
		this.getDispatchToken = this.getDispatchToken.bind(this);
		this.getSelectedNodeList = this.getSelectedNodeList.bind(this);
		this.getDataAtPath = this.getDataAtPath.bind(this);
		this.getOutput = this.getOutput.bind(this);
		this.getInput = this.getInput.bind(this);
		this.initHive(this.data);
	}

    // private:
	initHive(nodePlugData) {
		this.hive = new Hive();
		this.nodeSystem = new NodeSystem(nodePlugData, (nodeSystem) => {
			// initilized.
            this.hive.connect('ws://localhost:8080', '', true);
			this.emit(Constants.INITIALIZED, null);
		});
		this.hive.on(Hive.IMAGE_RECIEVED, (err, param, data) => {
			this.emit(Constants.IMAGE_RECIEVED, err, param, data);
		});
		this.nodeSystem.on(NodeSystem.SCRIPT_SERIALIZED, (script) => {
			//console.warn('SCRIPT>', script);
			this.hive.runScript(script);
		});
		this.nodeSystem.initEmitter(this);
	}

	/**
	 * shaderフォルダのファイル一覧を返す.
	 */
	getShaderList() {
		return this.hive.getShaderList();
	}

	/**
	 * dispatchTokenを返す.
	 */
	getDispatchToken() {
		return this.dispatchToken;
	}

	/**
	 * 全てのノードリストを返す
	 */
	getNodes(nodePath) {
		let path = this.data.nodePath;
		if (nodePath !== null && nodePath !== undefined) {
			path = nodePath;
		}

		let result = this.getDataAtPath(path);
		return result.nodes;
	}

	/**
	 * 全てのプラグリストを返す
	 */
	getPlugs(nodePath) {
		let path = this.data.nodePath;
		if (nodePath !== null && nodePath !== undefined) {
			path = nodePath;
		}

		let result = this.getDataAtPath(path);
		return result.plugs;
	}

	/**
	 * 指定したノード階層でのデータを返す
	 * @param path ノード階層を表す配列
	 */
	getDataAtPath(path) {
		let result = this.data;
		for (let i = 0; i < path.length; i = i + 1) {
			let p = path[i];
			let found = false;
			for (let k = 0; k < result.nodes.length; k = k + 1) {
				if (result.nodes[k].varname === p) {
					found = true;
					result = result.nodes[k];
					break;
				}
			}
			if (!found) { break; }
		}
		return result;
	}

	/**
	 * 現在のノード階層を返す.
	 */
	getNodePath() {
		return this.data.nodePath;
	}

	/**
	 * 現在のノード階層でのinputを返す.
	 */
	getInput() {
		let data = this.getDataAtPath(this.getNodePath());
		return data.input;
	}

	/**
	 * 現在のノード階層でのoutputを返す.
	 */
	getOutput() {
		let data = this.getDataAtPath(this.getNodePath());
		return data.output;
	}

	/**
	 * 特定のnodeとそのindexを返す.
	 */
	getNode(varname) {
		let nodes = this.getNodes();
		for (let i = 0; i < nodes.length; i = i + 1) {
			if (nodes[i].varname === varname) {
				return { node : nodes[i], index : i }
			}
		}
		return null;
	}

	/**
	 * ノード名リストを返す
	 */
	getNodeNameList() {
		let namelist = this.nodeSystem.GetNodeNameList();
		return namelist;
	}

	/**
	 * 初期ノード位置(固定)を返す
	 */
	getInitialNodePosition() {
		return ActionExecuter.initialData.node.pos;
	}

	/**
	 * 選択中のノードリストを返す.
	 */
	getSelectedNodeList() {
		let nodes = this.getNodes();
		let selected = [];
		for (let i = 0, size = nodes.length; i < size; i = i + 1) {
			if (nodes[i].select) {
				selected.push(nodes[i]);
			}
		}
		return selected;
	}
}
