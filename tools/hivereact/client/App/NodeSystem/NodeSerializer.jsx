

export default class NodeSerializer {
	constructor() {
	}

	serialize() {
		return "";
	}
    
    newNode(nodeinfo) {
        const definistion = nodeinfo.customfunc + '\n';
        const newInstance = nodeinfo.varname + " = " + nodeinfo.funcname + ".new()\n";
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
    
    
}
