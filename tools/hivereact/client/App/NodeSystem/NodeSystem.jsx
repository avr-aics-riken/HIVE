import NodeCreator from './NodeCreator.jsx'
import NodeSerializer from './NodeSerializer.jsx'
import EventEmitter from 'eventemitter3'
import Constants from '../Core/Constants.jsx'


function visit(node, nodelist) {
    let i;
    if (node.visited === false) {
        node.visited = true;
        for (i = 0; i <node.outputs.length; ++i) {
            visit(node.outputs[i], nodelist);
        }
        nodelist.unshift(node.node); // push to front
    }
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

function updateGraph(data) {

    const nodes = data.nodes;
    const plugs = data.plugs;

    let nodecache = {};
    let i;
    for (i = 0; i < nodes.length; ++i) {
        const n = nodes[i];
        const nc = {node: n, inputs:[], outputs:[], visited: false};
        nodecache[n.varname] = nc;
    }
    for (i = 0; i < plugs.length; ++i) {
        //console.log(plugs[i]);
        const p = plugs[i];
        const outnode = nodecache.hasOwnProperty(p.output.nodeVarname) ? nodecache[p.output.nodeVarname] : null;
        const inpnode = nodecache.hasOwnProperty(p.input.nodeVarname) ? nodecache[p.input.nodeVarname] : null;
		if (outnode && inpnode) {
	        inpnode.inputs.push(outnode);
	        outnode.outputs.push(inpnode);
		}
    }
    return nodecache;
}


export default class NodeSystem extends EventEmitter {

    constructor(nodePlugData, callback) {
        super();
        this.nodeSerializer = new NodeSerializer();
        this.nodeCreator = new NodeCreator(callback);
        this.data = nodePlugData;
        this.nodeGraph = {};
        this.nodeQueue = [];
    }

    doNodes() {
        this.nodeGraph = updateGraph(this.data);
        this.nodeQueue = topologicalSort(this.nodeGraph);
        let script = '';
        this.nodeQueue.forEach((node) => {
            script += this.nodeSerializer.doNode(node);
        });
        return script;
    }

    CreateNodeInstance(nodeName) {
        let srcNode = this.nodeCreator.GetNodeInfo(nodeName);
        if (srcNode === undefined) {
            console.error('Not found node template.');
            return;
        }

        let node = JSON.parse(JSON.stringify(srcNode)); // copy
        node.uiComponent = srcNode.uiComponent; // restore reference
        return node;
    }

    GetNodeNameList() {
        return this.nodeCreator.GetNodeNameList();
    }


    initEmitter(store) {

        store.on(Constants.NODE_INPUT_CHANGED, (err, data) => {
            console.log('NS catched:NODE_INPUT_CHANGED', err, data);
            let node = data;

            // set to instance
            //console.log('CHANGENODE->', node);
            let script = "print('NODE INPUT CHANGED!!', 'inst='," + node.varname + ")\n";

            script += this.nodeSerializer.updateNodeInput(node);
            script += this.doNodes();

            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
        });

        /*
        store.on(Constants.NODE_CHANGED, (err, data) => {
            // not need now.
        });
        store.on(Constants.PLUG_CHANGED, (err, data) => {
            // not need now.
        });
        store.on(Constants.NODE_COUNT_CHANGED, (err, data) => {
            console.log('NS catched:NODE_COUNT_CHANGED', err, data);
            // not need now.
        });
        store.on(Constants.NODE_INPUT_CHANGED, (err, data) => {
            // TODO
        });
        */

        store.on(Constants.NODE_ADDED, (err, data) => {
            // create new / delete instance
            console.log('NS catched:NODE_ADDED', err, data);

            const node = data;
            let script = this.nodeSerializer.newNode(node);
            script += this.doNodes();
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
        });
        store.on(Constants.NODE_DELETED, (err, data) => {
            console.log('NS catched:NODE_DELETED', err, data);

            let node = data;
            let script = this.nodeSerializer.deleteNode(node);
            script += this.doNodes();
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
        });
        store.on(Constants.PLUG_ADDED, (err, data) => {
            console.log('NS catched:PLUG_ADDED', err, data);

            const script = this.doNodes();
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
        });
        store.on(Constants.PLUG_DELETED, (err, data) => {
            console.log('NS catched:PLUG_DELETED', err, data);

            const script = this.doNodes();
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
        });
    }
}
NodeSystem.SCRIPT_SERIALIZED = "nodesystem_script_serialized";
