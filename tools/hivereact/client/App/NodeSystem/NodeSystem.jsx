import NodeCreator from './NodeCreator.jsx'
import NodeSerializer from './NodeSerializer.jsx'
import Store from '../Core/Store.jsx'

export default class NodeSystem {
    
    constructor(callback) {
        this.nodeCreator = new NodeCreator();
        this.nodeSerializer = new NodeSerializer();
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
        store.on(Store.NODE_CHANGED, () => {
            console.log('NS catched:NODE_CHANGED');    
        });
        store.on(Store.NODE_COUNT_CHANGED, () => {
            console.log('NS catched:NODE_COUNT_CHANGED');    
        });
        store.on(Store.IMAGE_RECIEVED, () => {
            console.log('NS catched:IMAGE_RECIEVED');            
        });
        store.on(Store.ADD_COMPONENT, () => {
            console.log('NS catched:ADD_COMPONENT');
        });
    }
}
