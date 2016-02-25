
function getData(url, callback) {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = (function(xhr) {
        if (this.readyState == 4) { // DONE
            if (this.status == 200) { // OK
                if (callback) {
                    callback(null, this.response);
                }
            } else {
                console.log("status = " + this.status);
            } 
        }    
    }).bind(xhr);
    xhr.open("GET", url);
    xhr.send();
}

export default class NodeCreator {
    
    constructor(callback) {

        this.initCallback = callback;
        this.nodeList = [];
        getData("http://localhost:8080/nodelist.json", (err, data) => {
            const jsondata = JSON.parse(data);
            console.log('nodedata=', jsondata);
            this.nodeList = jsondata;
            var n;
            for (n = 0; n < this.nodeList.length; ++n) {
                var uiFunc = this.nodeList[n].uiFunc;
                if (uiFunc) {
                    //console.log('before eval', uiFunc.length);
                    this.nodeList[n].uiComponent = eval(uiFunc);
                    //console.log(this.nodeList[n].uiComponent); // react class (same level with import uiComponent from XXX)
                    //console.log('after eval');
                }
            }
            if (this.initCallback) {
                this.initCallback(this);
            }
        });
    }

    // find node by name    
    findNodeByName(name) {
        var n;
        for (n = 0; n < this.nodeList.length; ++n) {
            if (this.nodeList[n].name === name) {
                break;
            } 
        }
        if (n === this.nodeList.length) {
            return undefined;
        }
        return this.nodeList[n];
    }

    // Temp    
    /*GetUIComponent(nodeName) {
        const node = this.findNodeByName(nodeName);
        if (node) {
            return node.uiComponent;
        } else {
            return null;
        }
    }*/
        
    GetNodeInfo(nodeName) {
        const node = this.findNodeByName(nodeName);
        return node;
    }
    
    GetNodeNameList() {
        var nlist = [];
        for (let n = 0; n < this.nodeList.length; ++n) {
            nlist.push(this.nodeList[n].name);
        }
        return nlist;
    }
    
}
