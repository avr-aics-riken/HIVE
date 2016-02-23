import NodeCreator from './NodeCreator.jsx'
import NodeSerializer from './NodeSerializer.jsx'
import Store from '../Core/Store.jsx'

export default class NodeSystem {
    
    constructor(callback) {
        this.nodeCreator = new NodeCreator();
        this.nodeSerializer = new NodeSerializer();
    }
    
    // Temp
    GetNodeInfo(nodeName) {
        return this.nodeCreator.GetNodeInfo(nodeName);
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
