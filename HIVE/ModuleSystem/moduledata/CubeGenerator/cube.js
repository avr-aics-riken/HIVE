//
// test code.
//

(function () {
    function  es5UI(dom, node) {
        console.log('create cubeUI', dom, node);
        var d = document.createElement('div');
        d.style.width = '128px';
        d.style.height = '128px';       
        d.style.backgroundColor = "#F00";       
        dom.appendChild(d);
    }
    return es5UI;
}());

