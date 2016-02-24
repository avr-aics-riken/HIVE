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
    
    GetNodeList() {
        return this.nodeCreator.GetNodeNameList();    
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
            // not need now.            
        });
        store.on(Store.NODE_COUNT_CHANGED, (err, data) => {
            console.log('NS catched:NODE_COUNT_CHANGED', err, data);
            // not need now.           
        });
        
        /*store.on(Store.NODE_INPUT_CHANGED, (err, data) => {
            // TODO
        });*/
        
        store.on(Store.NODE_ADDED, (err, data) => {
            // create new / delete instance
            console.log('NS catched:NODE_ADDED', err, data);

            let node = data;            
            this.nodeSerializer.newNode(node);
        });
        store.on(Store.NODE_DELETED, (err, data) => {
            console.log('NS catched:NODE_DELETED', err, data);
            
            let node = data;            
            this.nodeSerializer.deleteNode(node);
        });
        store.on(Store.PLUG_ADDED, (err, data) => {
            console.log('NS catched:PLUG_ADDED', err, data);
        
            // TODO
        });
        store.on(Store.PLUG_DELETED, (err, data) => {
            console.log('NS catched:PLUG_DELETED', err, data);
        
            // TODO
        });
    }
}
NodeSystem.SCRIPT_SERIALIZED = "nodesystem_script_serialized";
