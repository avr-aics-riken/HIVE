import EventEmitter from 'eventemitter3'
import Core from '../../Core'

function calcPlugPositionY(node, inoutIndex) {
	const holeSize = node.node.close ? 10 : 15;
	return node.node.pos[1] + (inoutIndex + 1) * (holeSize + 3) + 24
}

function calcSimplePlugPositionY(node) {
	return node.node.pos[1] + 18 + 20
}

function validatePlugInfo(plug, plugInfo, dataName) {
	if (dataName !== null && dataName !== undefined) {
		return (plug.nodeVarname === plugInfo.data.nodeVarname && dataName === plugInfo.data.name);
	} else {
		return (plug.nodeVarname === plugInfo.data.nodeVarname && plug.name === plugInfo.data.name);
	}
}

export default class Store extends EventEmitter {
	constructor(dispatcher, coreStore) {
		super();
		this.coreStore = coreStore;
		this.dispatchToken = dispatcher.register(this.actionHandler.bind(this));

		// 以下の形式のプラグ情報のリスト
		// {
		//   input : {
		//      pos : [x, y],
		//      nodeVarname : varname,
		//      name : name,
		//		nodeRef : 接続先のノードまたはグループ
		//   },
		//   output : {
		//      pos : [x, y],
		//      nodeVarname : varname,
		//      name : name,
		//		nodeRef : 接続先のノードまたはグループ
		//   },
		// }
		this.plugPosList = [];

		// 以下の形式の選択中の端子情報リスト
		// 端子は2つ以上選択できず、2つ選択されたときは接続されたものとみなす
		// {
		//   nodeVarname : nodevarname,
		//   data : 端子データ,
		//   isInput : 入力端子ならtrue
		// }
		this.selectedHoles = [];

		// ビューのズーム
		this.zoom = 1.0;

		// nodemap
		this.nodeMap = {};
		this.nodeToGroupMap = {};

		// { nodeVarname : {width:"", height:""} }
		this.nodeSizeMap = {};

		coreStore.on(Core.Constants.NODE_COUNT_CHANGED, (err, data) => {
			// ノードマップを作り直す.
			this.regenerateNodeMap();
			this.recalcPlugPosition();
			for (let n in this.nodeSizeMap) {
				if (!this.nodeMap.hasOwnProperty(n)) {
					delete this.nodeSizeMap[n];
				}
			}
			this.emit(Store.NODE_COUNT_CHANGED, null);
		});

		coreStore.on(Core.Constants.PLUG_COUNT_CHANGED, (err, data) => {
			// ノードマップを作り直す.
			this.regenerateNodeMap();
			this.recalcPlugPosition();
			this.emit(Store.PLUG_COUNT_CHANGED, err, this.plugPosList);
		});

		coreStore.on(Core.Constants.NODE_CLOSE_CHANGED, (err, data) => {
			this.recalcPlugPosition();
			this.emit(Store.NODE_CLOSE_CHANGED, err, data);
		});

		coreStore.on(Core.Constants.NODE_POSITION_CHANGED, (err, data) => {
			for (let i = 0, size = this.plugPosList.length; i < size; i = i + 1) {
				let plug = this.plugPosList[i];
				if (plug.input.nodeVarname === data.varname ||
					(plug.input.hasOwnProperty('nodeRef') && plug.input.nodeRef.varname === data.varname)) {
					let inpos = this.calcPlugPosition(true, plug, data);
					if (inpos) {
						plug.input.pos = inpos;
					}
				}
				if (plug.output.nodeVarname === data.varname ||
					(plug.output.hasOwnProperty('nodeRef') && plug.output.nodeRef.varname === data.varname)) {
					let outpos = this.calcPlugPosition(false, plug, data);
					if (outpos) {
						plug.output.pos = outpos;
					}
				}
			}
			this.emit(Store.PLUG_POSITION_CHANGED, null);
		});

		this.calcPlugPosition = this.calcPlugPosition.bind(this);
		this.recalcPlugPosition = this.recalcPlugPosition.bind(this);

		this.getPlugPosList = this.getPlugPosList.bind(this);
		this.changePlugPosition = this.changePlugPosition.bind(this);
		this.dragPlug = this.dragPlug.bind(this);
		this.endDragPlug = this.endDragPlug.bind(this);
		this.selectPlugHole = this.selectPlugHole.bind(this);
		this.unSelectPlugHoles = this.unSelectPlugHoles.bind(this);
		this.disconnectPlugHole = this.disconnectPlugHole.bind(this);
		this.changeZoom = this.changeZoom.bind(this);
		this.isConnected = this.isConnected.bind(this);
		this.isGroup = coreStore.isGroup;

		this.getCurrentNode = this.getCurrentNode.bind(this);
	}

	regenerateNodeMap() {
		let addNodeToNodeMap = (n) => {
			if (this.isGroup(n)) {
				this.nodeMap[n.varname] = n;
				this.nodeToGroupMap[n.varname] = n;
				for (let i = 0; i < n.nodes.length; i = i + 1) {
					addNodeToNodeMap(n.nodes[i]);
					this.nodeToGroupMap[n.nodes[i].varname] = n;
				}
			} else {
				this.nodeMap[n.varname] = n;
			}
		}
		this.nodeMap = {};
		this.nodeToGroupMap = {};
		for (let i = 0, size = this.coreStore.getNodes().length; i < size; i = i + 1) {
			let n = this.coreStore.getNodes()[i];
			this.nodeMap[n.varname] = n;
			//addNodeToNodeMap(n);
		}
	}

	// 現在のノード階層で該当するノードを返す。
	// 現在の階層にノードがない場合は、ノードを含んでいるグループが返る。
	// ノードが無い場合はnullを返す.
	getCurrentNode(varname) {
		let node = null;
		let nodes = this.coreStore.getNodes();
		if (this.nodeMap.hasOwnProperty(varname)) {
			node = this.nodeMap[varname];
		} else {
			for (let i = 0; i < nodes.length; i = i + 1) {
				if (this.coreStore.findNode(nodes[i], varname)) {
					node = nodes[i];
					break;
				}
			}
		}
		return node;
	}

	recalcPlugs() {
		let plugs = this.coreStore.getPlugs();
		for (let i = 0; i < plugs.length; i = i + 1) {
			let plug = plugs[i];
			let inNode = this.getCurrentNode(plug.input.nodeVarname);
			let outNode = this.getCurrentNode(plug.output.nodeVarname);
			if (inNode && outNode) {
				let plugPosition = {
					input : {
						nodeVarname : plug.input.nodeVarname,
						name : plug.input.name,
						pos : this.calcPlugPosition(true, plug, inNode),
						nodeRef : inNode
					},
					output : {
						nodeVarname : plug.output.nodeVarname,
						name : plug.output.name,
						pos : this.calcPlugPosition(false, plug, outNode),
						nodeRef : outNode
					}
				};
				if (!plugPosition.input.pos || !plugPosition.output.pos) {
					console.error(inNode, outNode, plugPosition);
				}
				//this.emit(Store.PLUG_POSITION_CHANGED, null,  plugPosition);
				this.plugPosList.push(plugPosition);
			}
		}
	}

	recalcGlobalInputs() {
		let globalIn = this.coreStore.getInput();
		for (let i = 0; i < globalIn.length; i = i + 1) {
			let data = globalIn[i];
			let node = this.getCurrentNode(data.nodeVarname);
			if (!node) {
				console.error("not found global plug node", data.nodeVarname, this.coreStore.data);
			}
			let plugPosition = {
				input : {
					name : data.name,
					nodeVarname : node.varname,
					nodeRef : node
				},
				output : {
					name : "",
					nodeVarname : "",
					pos : [-10000, node.node.pos[1]]
				}
			};
			plugPosition.input.pos = this.calcPlugPosition(true, plugPosition, node);
			//this.emit(Store.PLUG_POSITION_CHANGED, null,  plugPosition);
			this.plugPosList.push(plugPosition);
		}
	}

	recalcGlobalOutputs() {
		let globalOut = this.coreStore.getOutput();
		for (let i = 0; i < globalOut.length; i = i + 1) {
			let data = globalOut[i];
			let node = this.getCurrentNode(data.nodeVarname);
			if (!node) {
				console.error("not found global plug node", data.nodeVarname, this.coreStore.data);
			}
			let plugPosition = {
				input : {
					name : "",
					nodeVarname : "",
					pos : [10000, node.node.pos[1]]
				},
				output : {
					name : data.name,
					nodeVarname : node.varname,
					nodeRef : node
				}
			};
			plugPosition.output.pos = this.calcPlugPosition(false, plugPosition, node);
			//this.emit(Store.PLUG_POSITION_CHANGED, null,  plugPosition);
			this.plugPosList.push(plugPosition);
		}
	}

	recalcPlugPosition() {
		this.plugPosList = [];
		this.recalcPlugs.bind(this)();
		this.recalcGlobalInputs.bind(this)();
		this.recalcGlobalOutputs.bind(this)();
		this.emit(Store.PLUG_POSITION_CHANGED, null);
	}

	/**
	 * プラグ位置を計算して返す.
	 * @param isInput 入力端子かどうか
	 * @param plug プラグ。
	 * @param node プラグが接続されているノード
	 */
	calcPlugPosition(isInput, plug, node) {
		let isClosed = node.node.close;
		if (isInput) {
			if (plug.input.nodeVarname === node.varname || this.isGroup(node)) {
				let count = 0;
				for (let k = 0; k < node.input.length; k = k + 1) {
					if (Array.isArray(node.input[k].array)) {
						let inputArray = node.input[k].array;
						for (let n = 0; n < inputArray.length; n = n + 1) {
							if (inputArray[n].hasOwnProperty('hole') && !inputArray[n].hole) {
								continue;
							}
							if (inputArray[n].name === plug.input.name) {
								if (node.node.pos[0] === null) {
									console.error(node);
								}
								return [node.node.pos[0], calcPlugPositionY(node, count)];
							}
							if (isClosed) {
								if (this.isConnected(node.varname, inputArray[n].name)) {
									count = count + 1;
								}
							} else {
								count = count + 1;
							}
						}
					} else {
						if (node.input[k].hasOwnProperty('hole') && !node.input[k].hole) {
							continue;
						}
						if (node.input[k].name === plug.input.name) {
							if (node.node.pos[0] === null) {
								console.error(node);
							}
							return [node.node.pos[0], calcPlugPositionY(node, count)];
						}
						if (isClosed) {
							if (this.isConnected(node.varname, node.input[k].name)) {
								count = count + 1;
							}
						} else {
							count = count + 1;
						}
					}
				}
			}
			//return [100000, node.node.pos[1]];
		} else {
			if (plug.output.nodeVarname === node.varname || this.isGroup(node)) {
				let width = 200;
				if (this.nodeSizeMap.hasOwnProperty(node.varname)) {
					width = this.nodeSizeMap[node.varname].width;
				}
				for (let k = 0; k < node.output.length; k = k + 1) {
					if (node.output[k].name === plug.output.name) {
						return [node.node.pos[0] + width, calcPlugPositionY(node, k)];
					}
				}
			}
			//return [-100000, node.node.pos[1]];
		}
		//console.error("notcalculate", this.isGroup(node), isInput, plug, node)
		return null;
	}

	calcSimplePlugPosition(plug) {
		let pos = [[0,0], [0,0]];
		if (this.nodeMap.hasOwnProperty(plug.input.nodeVarname)) {
			let node = this.nodeMap[plug.input.nodeVarname];
			pos[0] = [node.node.pos[0], calcSimplePlugPositionY(node)];
		}
		if (this.nodeMap.hasOwnProperty(plug.output.nodeVarname)) {
			let node = this.nodeMap[plug.output.nodeVarname];
			let width = this.nodeSizeMap[plug.output.nodeVarname].width;
			pos[1] = [node.node.pos[0] + width, calcSimplePlugPositionY(node)];
		}
		return pos;
	}

	/**
	 * plug位置リストを返す.
	 */
	getPlugPosList() {
			return this.plugPosList;
	}

	/**
	 * ノードサイズをセット
	 */
	setNodeSize(varname, width, height) {
		this.nodeSizeMap[varname] = { width : width, height : height };
	}

	/**
	 * 選択中の端子リストを返す
	 */
	getSelectedPlugHoles() {
		return this.selectedHoles;
	}

	/**
	 * ズーム値を返す.
	 */
	getZoom() {
		return this.zoom;
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
					this[payload.actionType](payload);
				})();
			}
		}
	}

	/**
	 * プラグ位置を変更する.
	 * @private
	 */
	changePlugPosition(payload) {
		for (let i = 0; i < this.plugPosList.length; i = i + 1) {
			if (payload.isInput) {
				if (this.plugPosList[i].input.nodeVarname === payload.nodeVarname &&
					this.plugPosList[i].input.name === payload.name) {

					this.plugPosList[i].input.pos = JSON.parse(JSON.stringify(payload.pos));
					//this.emit(Store.PLUG_POSITION_CHANGED, null, this.plugPosList[i]);
				}
			} else {
				if (this.plugPosList[i].output.nodeVarname === payload.nodeVarname &&
					this.plugPosList[i].output.name === payload.name) {

					this.plugPosList[i].output.pos = JSON.parse(JSON.stringify(payload.pos));
					//this.emit(Store.PLUG_POSITION_CHANGED, null, this.plugPosList[i]);
				}
			}
		}
	}

	/// 入力端子にプラグが繋がっているかどうか返す
	isConnected(nodeVarname, inputName) {
		for (let i = 0; i < this.plugPosList.length; i = i + 1) {
			if (this.plugPosList[i].input.nodeRef.varname === nodeVarname) {
				if (Array.isArray(this.plugPosList[i].input.array)) {
					for (let k = 0; k < this.plugPosList[i].input.array.length; k = k + 1) {
						if (this.plugPosList[i].input.array[k].name === inputName) {
							return true;
						}
					}
				} else if (this.plugPosList[i].input.name === inputName) {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * プラグのドラッグを開始する.
	 */
	dragPlug(payload) {
		if (payload.hasOwnProperty('plugInfo')) {
			this.emit(Store.PLUG_DRAGGING, null, payload.plugInfo, payload.inputPos, payload.outputPos);
		}
	}

	/**
	 * プラグのドラッグを終了する.
	 */
	endDragPlug(payload) {
		if (payload.hasOwnProperty('plugInfo')) {
			this.emit(Store.PLUG_DRAG_END, null, payload.plugInfo, payload.inputPos, payload.outputPos);
		}
	}

	/**
	 * プラグ端子を選択する
	 */
	selectPlugHole(payload) {
		if (payload.hasOwnProperty('plugInfo')) {
			this.selectedHoles.push(payload.plugInfo);
			this.emit(Store.PLUG_HOLE_SELECTED, null, this.selectedHoles);
		}
	}

	/**
	 * プラグ端子の選択を解除する
	 */
	unSelectPlugHoles(payload) {
		this.selectedHoles = [];
	}

	/**
	 * プラグ端子の接続を解除
	 */
	disconnectPlugHole(payload) {
		if (payload.hasOwnProperty('plugInfo')) {
			let plugInfo = payload.plugInfo;
			console.log(plugInfo);
			for (let i = 0; i < this.plugPosList.length; i = i + 1) {
				if (payload.plugInfo.isInput) {
					if (Array.isArray(this.plugPosList[i].input.array)) {
						let inputArray = this.plugPosList[i].input.array;
						for (let k = 0; k < inputArray.length; k = k + 1) {
							let input = inputArray[k];
							if (validatePlugInfo(this.plugPosList[i].input, payload.plugInfo, input.name)) {
								this.emit(Store.PLUG_HOLE_DISCONNECTED, null, this.plugPosList[i]);
								break;
							}
						}
					} else {
						if (validatePlugInfo(this.plugPosList[i].input, payload.plugInfo)) {
							this.emit(Store.PLUG_HOLE_DISCONNECTED, null, this.plugPosList[i]);
							break;
						}
					}
				} else {
					if (validatePlugInfo(this.plugPosList[i].output, payload.plugInfo)) {
						this.emit(Store.PLUG_HOLE_DISCONNECTED, null, this.plugPosList[i]);
						break;
					}
				}
			}
			// 外部へ公開していた場合は公開を中止する。
			if (payload.plugInfo.isInput) {
				let inputs = this.coreStore.getInput();
				for (let i = 0; i < inputs.length; i = i + 1) {
					let input = inputs[i];

					if (input.nodeVarname === plugInfo.data.nodeVarname &&
						input.name === plugInfo.data.name) {

						this.emit(Store.GROUP_INPUT_DISCONNECTED, null, input);
						break;
					}
				}
			} else {
				let outputs = this.coreStore.getOutput();
				for (let i = 0; i < outputs.length; i = i + 1) {
					let output = outputs[i];
					if (output.nodeVarname === plugInfo.data.nodeVarname &&
						output.name === plugInfo.data.name) {

						this.emit(Store.GROUP_OUTPUT_DISCONNECTED, null, output);
						break;
					}
				}
			}
		}
	}

	/**
	 * ズーム値を変更.
	 */
	changeZoom(payload) {
		if (payload.hasOwnProperty('zoom')) {
			this.zoom = payload.zoom;
			this.emit(Store.ZOOM_CHANGED, null, this.zoom);
		}
	}
}
Store.PLUG_COUNT_CHANGED = "plug_count_changed";
Store.PLUG_POSITION_CHANGED = "plug_position_changed";
Store.PLUG_DRAGGING = "plug_dragging";
Store.PLUG_DRAG_END = "plug_drag_end";
Store.PLUG_HOLE_SELECTED = "plug_hole_selected";
Store.PLUG_HOLE_DISCONNECTED = "plug_hole_disconnected";
Store.NODE_MOVED = "node_moved";
Store.ZOOM_CHANGED = "zoom_changed";
Store.NEED_RERENDER = "need_rerender";
Store.GROUP_INPUT_DISCONNECTED = "group_input_disconnected";
Store.GROUP_OUTPUT_DISCONNECTED = "group_output_disconnected";
Store.NODE_CLOSE_CHANGED = "nodestore_node_close_changed";
