import EventEmitter from 'eventemitter3'
import Core from '../../Core'

export default class Store extends EventEmitter {
	constructor(dispathcer, coreStore) {
		super();
		this.dispatchToken = dispathcer.register(this.actionHandler.bind(this));

		// プラグ情報のリスト
		this.plugPositions = [];

		// ノード情報のリスト
		this.nodes = [];

		// 選択中のノード. キー: varname, 値：node
		this.selectNodes = {};

		coreStore.on(Core.Store.NODE_COUNT_CHANGED, (err, data) => {
			this.nodeMap = {};
			for (let i = 0, size = coreStore.getNodes().length; i < size; i = i + 1) {
				this.nodeMap[coreStore.getNodes()[i].varname] = coreStore.getNodes()[i];
			}
		});

		this.calcPlugPosition = this.calcPlugPosition.bind(this);

		coreStore.on(Core.Store.PLUG_COUNT_CHANGED, (err, data) => {
			console.log("PLUG COUNT CHANGED");
			let plugs = coreStore.getPlugs();
			console.log("PLUGS", plugs);
			this.plugPositions = [];
			for (let i = 0; i < plugs.length; i = i + 1) {
				let plug = plugs[i];
				let inNode = this.nodeMap[plug.input.nodeVarname];
				let outNode = this.nodeMap[plug.output.nodeVarname];
				let plugPosition = {
					input : {
						nodeVarname : plug.input.nodeVarname,
						name : plug.input.name,
						pos : this.calcPlugPosition(true, plug, inNode)
					},
					output : {
						nodeVarname : plug.output.nodeVarname,
						name : plug.output.name,
						pos : this.calcPlugPosition(false, plug, outNode)
					}
				};
				this.plugPositions.push(plugPosition);
			}
			this.emit(Store.PLUG_POSITION_CHANGED, null, this.plugPositions);
		});

		coreStore.on(Core.Store.NODE_COUNT_CHANGED, (err, data) => {
			this.nodes = [].concat(coreStore.getNodes());
			let names = {};
			for (let i = 0; i < this.nodes.length; i = i + 1) {
				names[this.nodes[i].varname] = 1;
			}
			for (let i in this.selectNodes) {
				if (this.selectNodes.hasOwnProperty(i)) {
					if (!names.hasOwnProperty(i)) {
						delete this.selectNodes[i];
					}
				}
			}
		});

		this.getPlugPositions = this.getPlugPositions.bind(this);
		this.changePlugPosition = this.changePlugPosition.bind(this);
		this.selectNode = this.selectNode.bind(this);
		this.unSelectNode = this.unSelectNode.bind(this);
		this.getSelectedNodes = this.getSelectedNodes.bind(this);
		this.moveNode = this.moveNode.bind(this);
	}

	/**
	 * プラグ位置を計算して返す.
	 * @param isInput 入力端子かどうか
	 * @param plug プラグ。
	 * @param node プラグが接続されているノード
	 */
	calcPlugPosition(isInput, plug, node) {
		if (isInput) {
			if (plug.input.nodeVarname === node.varname) {
				for (let k = 0; k < node.input.length; k = k + 1) {
					if (node.input[k].name === plug.input.name) {
						return [node.pos[0], node.pos[1] + (k + 1) * 18 + 20];
					}
				}
			}
		} else {
			if (plug.output.nodeVarname === node.varname) {
				for (let k = 0; k < node.output.length; k = k + 1) {
					if (node.output[k].name === plug.output.name) {
						return [node.pos[0] + 200 + 10, node.pos[1] + (k + 1) * 18 + 20];
					}
				}
			}
		}
		return null;
	}

	/**
	 * ノードが選択されているかどうか返す.
	 */
	isSelected(node) {
		if (this.selectNodes.hasOwnProperty(node.varname)) {
			return true;
		}
		return false;
	}

	/**
	 * 選択中のノード一覧を返す.
	 */
	getSelectedNodes() {
		return this.selectNodes;
	}

	/**
	 * plug位置リストを返す.
	 */
	getPlugPositions() {
		return this.plugPositions;
	}

	/**
	 * dispatchTokenを返す.
	 */
	getDispatchToken() {
		return this.dispatchToken;
	}

	/**
	 * アクションハンドラ
	 * @private
	 */
	actionHandler(payload) {
		if (payload && this.hasOwnProperty(payload.actionType)) {
			if (payload.hasOwnProperty("id") && payload.id === this.dispatchToken) {
				(() => {
					this[payload.actionType].bind(this)(payload);
				}());
			}
		}
	}

	/**
	 * プラグ位置を変更する.
	 * @private
	 */
	changePlugPosition(payload) {
	//console.log("changePlugPosition");
		for (let i = 0; i < this.plugPositions.length; i = i + 1) {
			if (payload.isInput) {
				if (this.plugPositions[i].input.nodeVarname === payload.nodeVarname &&
					this.plugPositions[i].input.name === payload.name) {

					this.plugPositions[i].input.pos = JSON.parse(JSON.stringify(payload.pos));
					this.emit(Store.PLUG_POSITION_CHANGED, null, this.plugPositions);
				}
			} else {
				if (this.plugPositions[i].output.nodeVarname === payload.nodeVarname &&
					this.plugPositions[i].output.name === payload.name) {

					this.plugPositions[i].output.pos = JSON.parse(JSON.stringify(payload.pos));
					this.emit(Store.PLUG_POSITION_CHANGED, null, this.plugPositions);
				}
			}
		}
	}

	/**
	 * ノードを選択する
	 */
	selectNode(payload) {
		if (payload.hasOwnProperty('node')) {
			this.selectNodes[payload.node.varname] = payload.node;
			this.emit(Store.NODE_SELECTE_CHANGED, null, this.selectNodes);
		}
	}

	/**
	 * ノードを選択する
	 */
	unSelectNode(payload) {
		if (payload.hasOwnProperty('node')) {
			if (this.selectNodes.hasOwnProperty(payload.node)) {
				delete this.selectNodes[payload.node];
			} else {
				this.selectNodes = {};
				if (payload.hasOwnProperty('excludeNode') && payload.excludeNode) {
					this.selectNodes[payload.excludeNode.varname] = payload.excludeNode;
				}
			}
			this.emit(Store.NODE_SELECTE_CHANGED, null, this.selectNodes);
		}
	}

	/**
	 * ノードを移動させる.
	 */
	moveNode(payload) {
		this.emit(Store.NODE_MOVED, null, payload.mv);
	}
}
Store.PLUG_POSITION_CHANGED = "plug_position_changed";
Store.NODE_SELECTE_CHANGED = "node_selected";
Store.NODE_MOVED = "node_moved";
