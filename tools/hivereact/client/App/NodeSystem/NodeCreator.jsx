
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
    
    constructor() {
        
        this.nodeList = [];
        getData("http://localhost:8080/nodelist.json", (err, data) => {
            //console.log('DATA=', data);
            const jsondata = JSON.parse(data);
            console.log('nodedata=', jsondata);
            this.nodeList = jsondata;
        });
    }
    
    Create(nodename) {
        return {};    
    }
    
    
    
}
