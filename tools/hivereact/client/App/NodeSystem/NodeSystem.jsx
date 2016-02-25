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

    GetNodeNameList() {
        return this.nodeCreator.GetNodeNameList();
    }


    initEmitter(store) {
        store.on(Store.NODE_CHANGED, (err, data) => {

        });
        store.on(Store.NODE_INPUT_CHANGED, (err, data) => {
            //console.log('NS catched:NODE_INPUT_CHANGED', err, data);
            let node = data;
            //let nodes = store.getNodes();

            // set to instance

            //console.log('NODES=', nodes, 'PLUGS=', plugs);
            console.log('CHANGENODE->', node);
            let script = "print('NODE INPUT CHANGED!!', 'inst='," + node.varname + ")\n";//this.nodeSerializer.writeNode(nodes); // generate


            const inputs = node.input;
            let i;

            for (i = 0; i < inputs.length; ++i) {
                let v = inputs[i];
                if (v.type === 'vec4') {
                    script += this.nodeSerializer.setPropertyVal4(node, v.name, v.value[0], v.value[1], v.value[2], v.value[3]);
                } else if (v.type === 'vec3') {
                    script += this.nodeSerializer.setPropertyVal3(node, v.name, v.value[0], v.value[1], v.value[2]);
                } else if(v.type === 'vec2') {
                    script += this.nodeSerializer.setPropertyVal2(node, v.name, v.value[0], v.value[1]);
                } else if(v.type === 'float') {
                    script += this.nodeSerializer.setPropertyVal(node, v.name, v.value);
                } else if(v.type === 'string') {
                    script += this.nodeSerializer.setPropertyString(node, v.name, v.value);
                } else {
                    // TODO: ex. RenderObject
                    //script += this.setPropertyVal(node, v.name, v.value);
                }
            }
            script += this.nodeSerializer.doNode(node);

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

            const node = data;
            const script = this.nodeSerializer.newNode(node);
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
        });
        store.on(Store.NODE_DELETED, (err, data) => {
            console.log('NS catched:NODE_DELETED', err, data);

            let node = data;
            const script = this.nodeSerializer.deleteNode(node);
            this.emit(NodeSystem.SCRIPT_SERIALIZED, script);
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
