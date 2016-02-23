import NodeCreator from './NodeCreator.jsx'
import NodeSerializer from './NodeSerializer.jsx'
import EventEmitter from 'eventemitter3'
import Store from '../Core/Store.jsx'

export default class NodeSystem extends EventEmitter {
    
    constructor(callback) {
        super();
        this.nodeSerializer = new NodeSerializer();
        this.nodeCreator = new NodeCreator(callback);
    }
    
    // Temp
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
    
    initEmitter(store) {
        store.on(Store.NODE_CHANGED, (err, data) => {
            //console.log('NS catched:NODE_CHANGED', err, data);
            let node = data;
            //let nodes = store.getNodes();
            
            // set to instance

            //console.log('NODES=', nodes, 'PLUGS=', plugs);
            console.log('CHANGENODE->', node);
            let script = "print('NODE CHANGED!!')";//this.nodeSerializer.writeNode(nodes); // generate
            
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);            
        });
        store.on(Store.PLUG_CHANGED, (err, data) => {
            
            let nodes = store.getNodes();
            let plugs = store.getPlugs();
            
            let script = "print('PLUG CHANGED!!')";//this.nodeSerializer.WriteAll(nodes, plugs); // generate
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
        });
        
        store.on(Store.NODE_COUNT_CHANGED, (err, data) => {
            console.log('NS catched:NODE_COUNT_CHANGED', err, data);
            
            // create new / delete instance    
        });
    }
}
NodeSystem.SCRIPT_SERIALIZED = "nodesystem_script_serialized";
