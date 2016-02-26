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
        nodelist.unshift(node); // push to front
    }
}

function initFrags(nodeGraph) {
    let i;
    
    // init flag
    for (i in nodeGraph) {
        if (nodeGraph.hasOwnProperty(i)) {
            nodeGraph[i].visited  = false;
            nodeGraph[i].executed = false;
        }
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

export default class NodeSystem extends EventEmitter {

    constructor(nodePlugData, callback) {
        super();
        this.nodeSerializer = new NodeSerializer();
        this.nodeCreator = new NodeCreator(callback);
        this.data = nodePlugData;
        this.nodeGraph = {};
        this.nodeQueue = [];
    }
    
    
    updateGraph() {
        const data  = this.data;
        const nodes = data.nodes;
        const plugs = data.plugs;

        let ng = this.nodeGraph;
        let i;
        for (i = 0; i < nodes.length; ++i) {
            const n = nodes[i];
            if (!ng.hasOwnProperty(n.varname)) {
                const nc = {node: n, inputs:[], outputs:[], inPlugs:[], needexecute: true, created: false};
                ng[n.varname] = nc;
            } else {
                // temporary clear
                ng[n.varname].inputs  = [];
                ng[n.varname].outputs = [];
                ng[n.varname].inPlugs = [];
            }
            
        }
        
        // override all
        //console.log('AAAAA', plugs);
        for (i = 0; i < plugs.length; ++i) {
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
    
    executeNode() {
        let script = '';
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
            }
        });
        return script;
    }

    doNodes() {
        this.updateGraph();
        initFrags(this.nodeGraph);        
        this.nodeQueue = topologicalSort(this.nodeGraph);
        return this.executeNode();
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
            //let script = "print('NODE INPUT CHANGED!!', 'inst='," + node.varname + ")\n";

            //script += this.nodeSerializer.updateNodeInput(node);
            //script += this.doNodes();
            
            this.nodeGraph[node.varname].needexecute = true;
            
            let script = this.doNodes();
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
            //let script = this.nodeSerializer.newNode(node);
            //script += this.doNodes();
            let script = this.doNodes();
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
        });
        store.on(Constants.NODE_DELETED, (err, data) => {
            console.log('NS catched:NODE_DELETED', err, data);

            let node = data;
            //let script = this.nodeSerializer.deleteNode(node);
            //script += this.doNodes();
            let script = this.doNodes();
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
        });
        store.on(Constants.PLUG_ADDED, (err, data) => {
            console.log('NS catched:PLUG_ADDED', err, data);

            const plug = data;
            this.nodeGraph[plug.input.nodeVarname].needexecute = true;
            
            const script = this.doNodes();
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
        });
        store.on(Constants.PLUG_DELETED, (err, data) => {
            console.log('NS catched:PLUG_DELETED', err, data);

            const plug = data;
            this.nodeGraph[plug.input.nodeVarname].needexecute = true;
            
            const script = this.doNodes();
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
        });
    }
}
NodeSystem.SCRIPT_SERIALIZED = "nodesystem_script_serialized";
