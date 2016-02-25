import EventEmitter from 'eventemitter3'
import Dispatcher from "./Dispatcher.jsx"
import Hive from "../HIVE"
import ActionExecuter from "./ActionExecutor.jsx"
import NodeSystem from "../NodeSystem"
import Constants from "./Constants.jsx"

export default class Store extends EventEmitter {
	constructor() {
		super();


		// 全てのノード
        //this.data = {
		  this.nodes = [];

		  // 全てのプラグ
		  this.plugs = [];
        //}
        
		this.actionExecuter = new ActionExecuter(this, this.data);
		this.dispatchToken = Dispatcher.register(this.actionHandler.bind(this));

        
        

		this.initHive = this.initHive.bind(this);
		this.getNodes = this.getNodes.bind(this);
		this.getNode = this.getNode.bind(this);
		this.getNodeNameList = this.getNodeNameList.bind(this);
		this.getDispatchToken = this.getDispatchToken.bind(this);

		this.initHive();
	}

	initHive() {
		let randomid = Math.floor(Math.random() * 10000);
		this.hive = new Hive();
		this.nodeSystem = new NodeSystem((nodeSystem) => {
			// initilized.
			this.hive.on(Hive.IMAGE_RECIEVED, (err, param, data) => {
				this.emit(Constants.IMAGE_RECIEVED, err, param, data);
			});
			this.nodeSystem.on(NodeSystem.SCRIPT_SERIALIZED, (script) => {
				//console.warn('SCRIPT>', script);
				this.hive.runScript(script);
			});
			//this.hive.connect('', 'ipc:///tmp/HiveUI_ipc_' + randomid, true);
			this.hive.connect('ws://localhost:8080', '', true);
			this.emit(Constants.INITIALIZED, null);
		});
		this.nodeSystem.initEmitter(this);
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
	getNodes() {
		return this.nodes;
	}

	/**
	 * 全てのプラグリストを返す
	 */
	getPlugs() {
		return this.plugs;
	}

	/**
	 * 特定のnodeとそのindexを返す.
	 */
	getNode(varname) {
		for (let i = 0; i < this.nodes.length; i = i + 1) {
			if (this.nodes[i].varname === varname) {
				return { node : this.nodes[i], index : i }
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
	 * アクションハンドラ
	 * @private
	 */
	actionHandler(payload) {
		if (payload && this.actionExecuter.hasOwnProperty(payload.actionType)) {
			if (payload.hasOwnProperty("id") && payload.id === this.dispatchToken) {
				(() => {
					this.actionExecuter[payload.actionType].bind(this)(payload);
				}());
			}
		}
	}
}
