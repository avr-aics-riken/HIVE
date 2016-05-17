import NodeSerializer from './NodeSerializer.jsx'
import EventEmitter from 'eventemitter3'
import Constants from '../Core/Constants.jsx'

const EXECUTE_NOTYET = "not yet"
const EXECUTE_NOTFOUND = "not found"


function visit(node, nodelist) {
    let i;
    if (node.visited === false) {
        node.visited = true;
        for (i = 0; i <node.outputs.length; ++i) {
            visit(node.outputs[i], nodelist);
        }
        nodelist.unshift(node); // push to front
    }
}

function initVisitFrags(nodeGraph) {
    let i;
    for (i in nodeGraph) {
        if (nodeGraph.hasOwnProperty(i)) {
            nodeGraph[i].visited  = false;
        }
    }
}

function initExecuteFrags(nodeGraph) {
    let i;
    for (i in nodeGraph) {
        if (nodeGraph.hasOwnProperty(i)) {
            nodeGraph[i].executed = false;
        }
    }
}

function initFrags(nodeGraph) {
    // init flag
    initVisitFrags(nodeGraph);
    initExecuteFrags(nodeGraph);
}

function topologicalSort(nodeGraph) {
    let nodelist = [];
    let i;
    for (i in nodeGraph) {
        if (nodeGraph.hasOwnProperty(i)) {
            if (nodeGraph[i].visited === false) {
                visit(nodeGraph[i], nodelist);
            }
        }
    }

    //console.log('SORTED:', nodelist);
    return nodelist;
}

export default class NodeExecutor extends EventEmitter {

    constructor(nodePlugData) {
        super();
        this.nodeSerializer = new NodeSerializer();
        this.data = nodePlugData;
        this.nodeGraph = {};
        this.nodeQueue = [];
		this.updateGraphRecursive = this.updateGraphRecursive.bind(this);
        this.interactiveMode = true;

        this.eventNodeInputChanged = this.eventNodeInputChanged.bind(this);
        this.eventNodePropertyChanged = this.eventNodePropertyChanged.bind(this);
        this.eventNodeAdded = this.eventNodeAdded.bind(this);
        this.eventNodeDeleted = this.eventNodeDeleted.bind(this);
        this.eventPlugAdded = this.eventPlugAdded.bind(this);
        this.eventPlugDeleted = this.eventPlugDeleted.bind(this);
    }

    setInteractiveMode(interactiveMode) {
        this.interactiveMode = interactiveMode; 
    }
	updateGraphRecursive(root) {
		let ng = this.nodeGraph;
		const nodes = root.nodes;
		const plugs = root.plugs;

		for (let i = 0; i < nodes.length; i = i + 1) {
			let n = nodes[i];
			if (n.hasOwnProperty('nodes') && n.hasOwnProperty('plugs')) {
				this.updateGraphRecursive(n);
			} else {
				if (!ng.hasOwnProperty(n.varname)) {
                    // node node
					const nc = {node: n, inputs:[], outputs:[], inPlugs:[], needexecute: true, created: false, exeState: EXECUTE_NOTYET};
					ng[n.varname] = nc;
				} else {
					// temporary clear
					ng[n.varname].inputs  = [];
					ng[n.varname].outputs = [];
					ng[n.varname].inPlugs = [];
				}
			}
		}

		for (let i = 0; i < plugs.length; ++i) {
			const p = plugs[i];
			const outnode = ng.hasOwnProperty(p.output.nodeVarname) ? ng[p.output.nodeVarname] : null;
			const inpnode = ng.hasOwnProperty(p.input.nodeVarname) ? ng[p.input.nodeVarname] : null;
			if (outnode && inpnode) {
				inpnode.inputs.push(outnode);
				inpnode.inPlugs.push(p);
				outnode.outputs.push(inpnode);
			}
		}
	}

    updateGraph() {
        const data  = this.data;
        const nodes = data.nodes;
        const plugs = data.plugs;

		this.updateGraphRecursive(data);
    }

    executeNode() {
        let script = 'local executedNode = {}\n';
        this.nodeQueue.forEach((nd) => {
            let inputUpdate = false;
            let i
            let p

            // new node?
            if (nd.created === false) {
                nd.created = true;
                script += this.nodeSerializer.newNode(nd.node);
            }

            // child executed check
            for (i = 0; i < nd.inputs.length; ++i) {
                if (nd.inputs.hasOwnProperty(i) && nd.inputs[i].executed) {
                    inputUpdate = true;
                }
            }

            nd.needexecute |= inputUpdate;
            if (nd.needexecute) {
                script += this.nodeSerializer.updateNodeInput(nd.node);
                for (p = 0; p < nd.inPlugs.length; ++p) {
                    script += this.nodeSerializer.updateConnectedNodeInput(nd.inPlugs[p]);
                }
                script += this.nodeSerializer.doNode(nd.node);
                nd.needexecute = false;
                nd.executed = true;
                nd.exeState = "running";
            }
        });
        script += "collectgarbage('collect')\n";
        script += "\nreturn {doState=executedNode}\n";
        return script;
    }

    updateExecuteState(data) {
        if (data.hasOwnProperty('doState')) {
            let i;
            for (i in data.doState) {
                this.nodeGraph[i].exeState = data.doState[i];
            }
            //console.log('hoge', this.nodeGraph);
        }
    }

    getNodeExecutionState(varname) {
        if (this.nodeGraph.hasOwnProperty(varname)) {
            return this.nodeGraph[varname].exeState;
        } else {
            return EXECUTE_NOTFOUND;
        }
    }


    doNodes() {
        this.updateGraph();
        initFrags(this.nodeGraph);
        this.nodeQueue = topologicalSort(this.nodeGraph);
        return this.executeNode();
    }

    getExecutionOrder() {
        return this.nodeQueue; // cached order

        // calc order
        /*
        this.updateGraph(this.nodeGraph);
        initVisitFrags(this.nodeGraph);
        return topologicalSort(this.nodeGraph);
        */
    }

    getOrderByVarname(varnameArray) {
        let nodeQ = this.getExecutionOrder();
        console.log('Q', nodeQ);
        let i, order = {};
		let varnameToIndex = {};
        for (i = 0; i < nodeQ.length; i = i + 1) {
			let vname = nodeQ[i].node.varname;
			varnameToIndex[vname] = i;
		}
        for (i in nodeQ) {
            let vname = nodeQ[i].node.varname;
            let p = varnameToIndex[vname];
            if (p !== -1) {
                order[vname] = p;
            }
        }
        return order;
    }

    eventExecute() {
        if (this.interactiveMode) {            
            let script = this.doNodes();
            this.emit(NodeExecutor.SCRIPT_SERIALIZED, script);
        }
    }

    /* ------- emitter  event functions ------------- */    
    eventNodeInputChanged (err, data) {
        console.log('NS catched:NODE_INPUT_CHANGED', err, data);
        let node = data;

        this.eventExecute();        
    }

    eventNodePropertyChanged (err, data, prop) {
        console.log('NS catched:NODE_INPUT_PROPERTY_CHANGED', err, data, prop);
        let node = data;
        let p = prop;

        if (p.funcinput !== false) {
            this.nodeGraph[node.varname].needexecute = true;
        }

        // !! don't execute here.
        //// this.eventExecute();
    }

    eventNodeAdded(err, data) {
        // create new / delete instance
        console.log('NS catched:NODE_ADDED', err, data);

        this.eventExecute();        
    }
    eventNodeDeleted(err, data) {
        console.log('NS catched:NODE_DELETED', err, data);

        this.eventExecute();        
    }
    eventPlugAdded(err, data) {
        console.log('NS catched:PLUG_ADDED', err, data);

        const plug = data;
        this.nodeGraph[plug.input.nodeVarname].needexecute = true;

        this.eventExecute();        
    }
    eventPlugDeleted(err, data) {
        console.log('NS catched:PLUG_DELETED', err, data);

        const plug = data;
        this.nodeGraph[plug.input.nodeVarname].needexecute = true;

        this.eventExecute();        
    }
    /*-----------------------*/
    
    initEmitter(store) {
        store.on(Constants.NODE_INPUT_CHANGED, this.eventNodeInputChanged);
        store.on(Constants.NODE_INPUT_PROPERTY_CHANGED, this.eventNodePropertyChanged);
        store.on(Constants.NODE_ADDED, this.eventNodeAdded);
        store.on(Constants.NODE_DELETED, this.eventNodeDeleted);
        store.on(Constants.PLUG_ADDED, this.eventPlugAdded);
        store.on(Constants.PLUG_DELETED, this.eventPlugDeleted);
    }
    offEmitter(store) {
        store.off(Constants.NODE_INPUT_CHANGED, this.eventNodeInputChanged);
        store.off(Constants.NODE_INPUT_PROPERTY_CHANGED, this.eventNodePropertyChanged);
        store.off(Constants.NODE_ADDED, this.eventNodeAdded);
        store.off(Constants.NODE_DELETED, this.eventNodeDeleted);
        store.off(Constants.PLUG_ADDED, this.eventPlugAdded);
        store.off(Constants.PLUG_DELETED, this.eventPlugDeleted);
    }
}
NodeExecutor.SCRIPT_SERIALIZED = "nodesystem_script_serialized";
