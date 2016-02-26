

export default class NodeSerializer {
	constructor() {
	}

	serialize() {
		return "";
	}
    
    newNode(nodeinfo) {
        const definistion = nodeinfo.customfunc + '\n';
        const newInstance = nodeinfo.varname + " = " + nodeinfo.funcname + ".new('" + nodeinfo.varname + "')\n";
        const com = 'print("newinstace!>' + nodeinfo.varname + ' = ",' + nodeinfo.varname + ')\n'
        const src = definistion + newInstance + com;
        return src;                
    }
    
    deleteNode(nodeinfo) {
        const deleteInstance = nodeinfo.varname + " = nil\n"
        const src = deleteInstance
        return src;
    }
    
    doNode(nodeinfo) {
        const doInstance = nodeinfo.varname + ":Do()\n"
        const com = 'print("Do!>' + nodeinfo.varname + '")\n'
        const src = com + doInstance
        return src;
    }
    
    setPropertyString(nodeinfo, name, value) {
        const setsrc = nodeinfo.varname + ":Set('" + name + "','" + value + "')\n"
        return setsrc;
    }
    setPropertyVal(nodeinfo, name, value) {
        const setsrc = nodeinfo.varname + ":Set('" + name + "'," + value + ")\n"
        return setsrc;
    }
    setPropertyVal2(nodeinfo, name, x, y) {
        const setsrc = nodeinfo.varname + ":Set('" + name + "',{" + x + "," + y + "})\n"
        return setsrc;
    }
    setPropertyVal3(nodeinfo, name, x, y, z) {
        const setsrc = nodeinfo.varname + ":Set('" + name + "',{" + x + "," + y + "," + z + "})\n"
        return setsrc;
    }
    setPropertyVal4(nodeinfo, name, x, y, z, a) {
        const setsrc = nodeinfo.varname + ":Set('" + name + "',{" + x + "," + y + "," + z + "," + a + "})\n"
        return setsrc;
    }
    clearConnect(nodeinfo) {
        const clrsrc = nodeinfo.varname + ":ClearConnect()\n"
        return clrsrc;
    }
    
    updateNodeInput(node) {
        let script = this.clearConnect(node);
        const inputs = node.input;
        let i;
        for (i = 0; i < inputs.length; ++i) {
            let v = inputs[i];
            if (v.type === 'vec4') {
                script += this.setPropertyVal4(node, v.name, v.value[0], v.value[1], v.value[2], v.value[3]);
            } else if (v.type === 'vec3') {
                script += this.setPropertyVal3(node, v.name, v.value[0], v.value[1], v.value[2]);
            } else if(v.type === 'vec2') {
                script += this.setPropertyVal2(node, v.name, v.value[0], v.value[1]);
            } else if(v.type === 'float') {
                script += this.setPropertyVal(node, v.name, v.value);
            } else if(v.type === 'string') {
                script += this.setPropertyString(node, v.name, v.value);
            } else {
                // TODO: ex. RenderObject
                //script += this.setPropertyVal(node, v.name, v.value);
            }
        }
        return script;
    }
    
    updateConnectedNodeInput(plug) {
        let script = '';
        script += plug.input.nodeVarname + ":Set('" + plug.input.name + "'," + plug.output.nodeVarname + ":" + plug.output.name + "())\n";
        //console.error('updateConnectedNodeInput', script);
        return script; 
    }    
}
