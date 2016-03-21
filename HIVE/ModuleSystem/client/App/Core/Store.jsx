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
			varname : "Root",
            nodes : [],   // 全てのノード
            plugs : [],   // 全てのプラグ
			input : [],   // シーンの入力端子
			output : [],  // シーンの出力端子
			nodePath : [], // 表示しているノード階層のパス
			timeline : {
				frame : 0,
		        data : {
				/*
		            contents: [{
		                name: "test",
			            nodeVarname: "testvarname",
		                closed: false,
		                color: "rgb(32, 96, 196)",
		                propColor: "rgba(8, 62, 162, 1.0)",
		                props: [{
		                    name: "hogehoge",
				            nodeVarname: "testvarname",
		                    data: {
		                        10: 100,
		                        20: 120,
		                        25: 150
		                    }
		                }, {
		                    name: "piropiro",
				            nodeVarname: "testvarname",
		                    data: {
		                        30: 100,
		                        50: 220,
		                        95: 150
		                    }
		                }]
		            }, {
		                name: "munimuni",
			            nodeVarname: "groupvarname",
		                closed: false,
		                color: "rgb(196, 32, 64)",
		                propColor: "rgba(162, 8, 40, 1.0)",
		                props: [{
		                    name: "piropiro",
				            nodeVarname: "hogevarname",
		                    data: {
		                        5: 100,
		                        40: 220,
		                        60: 150
		                    }
		                }, {
		                    name: "piropiro2",
				            nodeVarname: "piyovarname",
		                    data: {
		                        15: 100,
		                        42: 220,
		                        64: 150
		                    }
		                }],
		            }]
					*/
		        }
			}
        }

		this.actionExecuter = new ActionExecuter(this);
        this.dispatchToken = Dispatcher.register(this.actionExecuter.actionHandler.bind(this.actionExecuter));

		this.initHive = this.initHive.bind(this);
		this.isGroup = this.isGroup.bind(this);
		this.getNodes = this.getNodes.bind(this);
		this.getNode = this.getNode.bind(this);
		this.getNodeNameList = this.getNodeNameList.bind(this);
		this.getDispatchToken = this.getDispatchToken.bind(this);
		this.getSelectedNodeList = this.getSelectedNodeList.bind(this);
		this.getDataAtPath = this.getDataAtPath.bind(this);
		this.getOutput = this.getOutput.bind(this);
		this.getInput = this.getInput.bind(this);
		this.getRootNode = this.getRootNode.bind(this);
		this.getRootNodes = this.getRootNodes.bind(this);
		this.getRootPlugs = this.getRootPlugs.bind(this);
		this.findNode = this.findNode.bind(this);
		this.hasCurrentKeyFrame = this.hasCurrentKeyFrame.bind(this);
		this.getTimelineContent = this.getTimelineContent.bind(this);
		this.initHive(this.data);
		this.findGroup = this.findGroup.bind(this);
		this.getTimelineName = this.getTimelineName.bind(this);
	}

    // private:
	initHive(nodePlugData) {
		this.hive = new Hive();
		this.hive.connect('ws://localhost:8080');//, '', true);

        this.nodeExecutor = new NodeSystem.NodeExecutor(nodePlugData);
        this.nodeCreator =new NodeSystem.NodeCreator("http://localhost:8080/modulelist.json", () => {
			this.emit(Constants.INITIALIZED, null);
		});
		this.hive.on(Hive.IMAGE_RECIEVED, (err, param, data) => {
			this.emit(Constants.IMAGE_RECIEVED, err, param, data);
		});
        this.hive.on(Hive.RENDERER_LOG_RECIEVED, (data) => {
			this.emit(Constants.RENDERER_LOG_RECIEVED, data);
		});
        this.hive.on(Hive.ANALYZED_DATA_RECIEVED, (data) => {
			this.emit(Constants.ANALYZED_DATA_RECIEVED, data);
        });
		this.nodeExecutor.on(NodeSystem.NodeExecutor.SCRIPT_SERIALIZED, (script) => {
			//console.warn('SCRIPT>', script);

            /*
            // Node execution order by varname
            let varnames = [];
            let i;
            for (i in this.data.nodes) {
                varnames.push(this.data.nodes[i].varname);
            }
            console.log('Order=', this.nodeExecutor.getOrderByVarname(varnames));
            */
			this.emit(Constants.SCRIPT_STATUS_CHANGED, null);

			this.hive.runScript(script, (err, data) => {
                if (err) {
                    return;
                }
                this.nodeExecutor.updateExecuteState(data);
				this.emit(Constants.SCRIPT_STATUS_CHANGED, null);
            });
		});
		this.nodeExecutor.initEmitter(this);
	}

	/**
	 * グループノードかどうか返す.
	 */
	isGroup(node) {
		return (node.hasOwnProperty('nodes') && node.hasOwnProperty('plugs'));
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
	 * 現在のノード階層のノードリストを返す
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
	 * ルート階層のノードリストを返す
	 */
	getRootNodes() {
		return this.getNodes([]);
	}

	/**
	 * ルートノードを返す
	 */
	getRootNode() {
		return this.data;
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
	 * ルート階層のプラグリストを返す
	 */
	getRootPlugs() {
		return this.getPlugs([]);
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
	 * 指定したノード階層でのinputを返す.
	 */
	getInput(nodePath) {
		let path = this.data.nodePath;
		if (nodePath !== null && nodePath !== undefined) {
			path = nodePath;
		}

		let result = this.getDataAtPath(path);
		return result.input;
	}

	/**
	 * 指定したノード階層でのoutputを返す.
	 */
	getOutput(nodePath) {
		let path = this.data.nodePath;
		if (nodePath !== null && nodePath !== undefined) {
			path = nodePath;
		}

		let result = this.getDataAtPath(path);
		return result.output;
	}

	/**
	 * 特定のnodeとそのindexを返す.
	 */
	getNode(varname, isRecursive) {
		let nodes = this.getNodes();
		for (let i = 0; i < nodes.length; i = i + 1) {
			if (nodes[i].varname === varname) {
				return { node : nodes[i], index : i }
			}
		}
		return null;
	}

	/**
	 * ノードを探してnodeを返す.
	 */
	findNode(rootNode, varname) {
		if (this.isGroup(rootNode)) {
			let nodes = rootNode.nodes;
			for (let i = 0; i < nodes.length; i = i + 1) {
				let n = nodes[i];
				if (n.varname === varname) {
					return n;
				}
				let result = this.findNode(n, varname);
				if (result) {
					return result;
				}
			}
		} else if (rootNode.varname === varname) {
			return rootNode;
		}
		return null;
	}

	/**
	 * あるvarnameのノードを含む現在の階層のグループを返す. なければnullを返す
	 */
	findGroup(varname) {
		let nodes = this.getNodes();
		for (let i = 0; i < nodes.length; i = i + 1) {
			if (this.isGroup(nodes[i]) && this.findNode(nodes[i], varname)) {
				return nodes[i];
			}
		}
		return null;
	}

	/**
	 * ノード名リストを返す
	 */
	getNodeNameList() {
		let namelist = this.nodeCreator.GetNodeNameList();
		return namelist;
	}

	/**
	 * ノードの実行状態を返す
	 */
    getNodeExecutionState(varname) {
        return this.nodeExecutor.getNodeExecutionState(varname);
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

	/**
	 * 現在のフレーム番号を返す
	 */
	getCurrentFrame() {
		return this.data.timeline.frame;
	}

	/**
	 * タイムラインのデータを返す.
	 */
	getTimelineData() {
		return this.data.timeline.data;
	}

	/**
	 * タイムラインのコンテンツを返す.
	 */
	getTimelineContent(nodeVarname) {
		let data = this.data.timeline.data;
		if (data.hasOwnProperty('contents')) {
			for (let i = 0; i < data.contents.length; i = i + 1) {
				if (data.contents[i].nodeVarname  === nodeVarname) {
					return data.contents[i];
				}
			}
		}
		return null;
	}

	/**
	 * タイムラインの名前を返す
	 */
	getTimelineName(content) {
		let n = this.getNode(content.nodeVarname);
		if (n) {
			return n.node.label ? n.node.label : n.node.name;
		} else {
			let group = this.findGroup(content.nodeVarname);
			let node = this.findNode(group, content.nodeVarname);
			if (node) {
				let gname = group.label ? group.label : group.name;
				let nname = node.label ? node.label : node.name;
				return gname + " - " + nname;
			}
		}
		return ""
	}

	/**
	 * inputが現在のフレームでキーを持っているか返す
	 */
	hasCurrentKeyFrame(input) {
		const currentFrame = this.getCurrentFrame();
		let content = this.getTimelineContent(input.nodeVarname);
		if (content) {
			if (content.nodeVarname  === input.nodeVarname) {
				let props = content.props;
				for (let k = 0; k < props.length; k = k + 1) {
					if (props[k].name === input.name && props[k].data.hasOwnProperty(currentFrame)) {
						return true;
					}
				}
			}
		}
		return false;
	}

}
