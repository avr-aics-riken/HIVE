
function setPropertyBool(nodeinfo, name, value) {
    const setsrc = nodeinfo.varname + ":Set('" + name + "'," + value + ")\n"
    return setsrc;
}
function setPropertyString(nodeinfo, name, value) {
    const setsrc = nodeinfo.varname + ":Set('" + name + "',\"" + value + "\")\n"
    return setsrc;
}
function setPropertyVal(nodeinfo, name, value) {
    const setsrc = nodeinfo.varname + ":Set('" + name + "'," + value + ")\n"
    return setsrc;
}
function setPropertyVal2(nodeinfo, name, x, y) {
    const setsrc = nodeinfo.varname + ":Set('" + name + "',{" + x + "," + y + "})\n"
    return setsrc;
}
function setPropertyVal3(nodeinfo, name, x, y, z) {
    const setsrc = nodeinfo.varname + ":Set('" + name + "',{" + x + "," + y + "," + z + "})\n"
    return setsrc;
}
function setPropertyVal4(nodeinfo, name, x, y, z, a) {
    const setsrc = nodeinfo.varname + ":Set('" + name + "',{" + x + "," + y + "," + z + "," + a + "})\n"
    return setsrc;
}
function setPropertyArray(nodeinfo, name, value) {
    var arraystr = '{';
    var i;
    for (i = 0; i <value.length; ++i) {
        arraystr += value[i] + ','; 
    }
    arraystr += '}';
    const setsrc = nodeinfo.varname + ":Set('" + name + "'," + arraystr + ")\n"
    return setsrc;
}    

function setPropertyOriginalArray(nodeinfo, input) {
    var arraystr = '{';
    var i, v;
    for (i = 0; i < input.array.length; ++i) {
        v = input.array[i];
        arraystr += "'" + v.value + "'" + ","; 
    }
    arraystr += '}';
    const setsrc = nodeinfo.varname + ":Set('" + input.name + "'," + arraystr + ")\n"
    return setsrc;
}

//-----

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
        const doInstance = 'executedNode["' + nodeinfo.varname + '"] = ' + nodeinfo.varname + ":Do()\n"
        const com = 'print("Do!>' + nodeinfo.varname + '")\n'
        const src = com + doInstance
        return src;
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
            if (v.hasOwnProperty('funcinput') && v.funcinput == false) {
                continue;
            }
            if(v.hasOwnProperty('array') && v.type === "string") {
                script += setPropertyOriginalArray(node, v);           
            } else if (v.type === 'vec4') {
                script += setPropertyVal4(node, v.name, v.value[0], v.value[1], v.value[2], v.value[3]);
            } else if (v.type === 'vec3') {
                script += setPropertyVal3(node, v.name, v.value[0], v.value[1], v.value[2]);
            } else if(v.type === 'vec2') {
                script += setPropertyVal2(node, v.name, v.value[0], v.value[1]);
            } else if(v.type === 'float') {
                script += setPropertyVal(node, v.name, v.value);
            } else if(v.type === 'int' || v.type === 'select') {
                script += setPropertyVal(node, v.name, v.value);
            } else if(v.type === 'string') {
                script += setPropertyString(node, v.name, v.value);
            } else if(v.type === 'bool') {
                script += setPropertyBool(node, v.name, v.value);            
            } else if(v.type === 'floatarray' || v.type === 'intarray') {
                script += setPropertyArray(node, v.name, v.value);                        
            } else {
                // TODO: ex. RenderObject
                //script += this.setPropertyVal(node, v.name, v.value);
            }
        }
        return script;
    }
    
    updateConnectedNodeInput(plug) {
        let script = '';
        if (plug.output.name.indexOf("[") >= 0) {
            var nameIndex = plug.output.name.split("[");
            var name = nameIndex[0];
            var index = nameIndex[1].split("]").join("");
            script += plug.input.nodeVarname + ":Connect('" + plug.input.name + "'," + plug.output.nodeVarname + ":" + name + "(" + index +"))\n";
        } else {
            script += plug.input.nodeVarname + ":Connect('" + plug.input.name + "'," + plug.output.nodeVarname + ":" + plug.output.name + "())\n";   
        }
        //console.error('updateConnectedNodeInput', script);
        return script; 
    }    
}
