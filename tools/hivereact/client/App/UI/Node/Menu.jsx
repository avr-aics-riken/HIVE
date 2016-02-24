import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'
import Store from './Store.jsx'

export default class Menu extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes : null,
        };

        // this.nodeChanged = this.nodeChanged.bind(this);
        // this.selectChanged = this.selectChanged.bind(this);
        // this.moveNode = this.moveNode.bind(this);
        //
        // this.componentDidMount = this.componentDidMount.bind(this);
        // this.componentWillUnmount = this.componentWillUnmount.bind(this);
        // this.onMouseMove = this.onMouseMove.bind(this);
        // this.onMouseUp = this.onMouseUp.bind(this);
        // this.onMouseDown = this.onMouseDown.bind(this);
        // this.onKeyDown = this.onKeyDown.bind(this);
        // this.onKeyUp = this.onKeyUp.bind(this);
        this.styles = this.styles.bind(this);
    }

    // componentDidMount() {
    //     window.addEventListener('mousemove', this.onMouseMove);
    //     window.addEventListener('mouseup', this.onMouseUp);
    //     window.addEventListener('keydown', this.onKeyDown);
    //     window.addEventListener('keyup', this.onKeyUp);
    //     this.props.store.on(Core.Store.NODE_CHANGED, this.nodeChanged);
    //     this.props.nodeStore.on(Store.NODE_SELECTE_CHANGED, this.selectChanged);
    //     this.props.nodeStore.on(Store.NODE_MOVED, this.moveNode);
    // }
    //
    // componentWillUnmount() {
    //     window.removeEventListener('mousemove', this.onMouseMove);
    //     window.removeEventListener('mouseup', this.onMouseUp);
    //     window.removeEventListener('keydown', this.onKeyDown);
    //     window.removeEventListener('keyup', this.onKeyUp);
    //     this.props.store.removeListener(Core.Store.NODE_CHANGED, this.nodeChanged);
    //     this.props.nodeStore.removeListener(Store.NODE_SELECTE_CHANGED, this.selectChanged);
    //     this.props.nodeStore.removeListener(Store.NODE_MOVED, this.moveNode);
    // }

    functionReplacer(data){
        return JSON.stringify(data, function(key, val){
            if(typeof val === "function"){
                return val.toString();
            }
            return val;
        }, 2);
    }

    allClearNode(){
        if(confirm('really?')){
            let nodes = this.props.store.getNodes();
            let plugs = this.props.store.getPlugs();
            // for(let i = plugs.length - 1; i >= 0; --i){
            //     this.props.action.deletePlug(plugs[i]);
            // }
            for(let i = nodes.length - 1; i >= 0; --i){
                this.props.action.deleteNode(nodes[i].varname);
            }
            return true;
        }else{
            return false;
        }
    }

    styles() {
        return {
            menuArea: {
                backgroundColor: "#222",
                color: "#eee",
                fontSize: "9pt",
                margin: "0px",
                padding: "0px",
                width: "100%",
                minHeight: "50px",
                position: "absolute",
                bottom: "0px",
                left: "0px"
            },
            block: {
                textAlign: "right",
                margin: "0px",
                padding: "2px 0px",
                width: "100%",
                minHeight: "20px",
                boxShadow: "0px 0px 1px 0px white inset"
            },
            blockTitle: {
                backgroundColor: "rgba(0, 0, 0, 0.5)",
                borderRadius: "0px 0px 15px 0px",
                color: "whitesmoke",
                fontSize: "x-small",
                lineHeight: "15px",
                margin: "0px",
                padding: "0px 12px 0px 5px",
                height: "15px",
                display: "inline-block",
                float: "left",
                boxShadow: "0px -1px 1px 1px #666 inset"
            }
        }
    }

    saveButton(value, script){
        return (
            <div><input type="button" value={value} onClick={script} /></div>
        );
    }

    loadButton(value, script){
        return (
            <div><input type="file" name={value} onChange={script} /></div>
        );
    }

    clearButton(value, script){
        return (
            <div><input type="button" value={value} onClick={script} /></div>
        );
    }

    block(value, key){
        const style = this.styles();
        if(value.title === ''){return;}
        return (
            <div style={style.block} key={key}>
                <span style={style.blockTitle}>{value.title}</span>
                {value.item(value.title, value.script)}
            </div>
        );
    }

    // bl という配列の中で定義した配列のとおりに
    // ブロックが生成される感じ（仮）
    render(){
        const style = this.styles();
        let bl = [
            {
                item: this.loadButton.bind(this),
                title: 'import',
                script: function(eve){
                    if(eve.currentTarget.files && eve.currentTarget.files.length > 0){
                        if(this.allClearNode.bind(this)()){
                            var reader = new FileReader();
                            reader.onload = function(){
                                let data = (JSON.parse(reader.result));
                                if(data.nodes && data.nodes.length > 0){
                                    for(let i in data.nodes){
                                        data.nodes[i].uiComponent = eval('(' + data.nodes[i].uiComponent + ')');
                                        this.props.action.addNode(data.nodes[i]);
                                    }
                                }else{
                                    console.log('import failed: data.length === 0');
                                }
                            }.bind(this);
                            reader.readAsText(eve.currentTarget.files[0]);
                        }
                    }
                }.bind(this)
            },
            {
                item: this.saveButton.bind(this),
                title: 'export',
                script: function(){
                    var data = {
                        nodes: this.props.store.getNodes(),
                        plugs: this.props.store.getPlugs()
                    };
                    var blob = new Blob([this.functionReplacer.bind(this)(data)], {type: "text/plain;charset=utf-8"});
                    saveAs(blob, "save.json");
                }.bind(this)
            },
            {
                item: this.clearButton.bind(this),
                title: 'all clear',
                script: this.allClearNode.bind(this)
            }
        ];
        return (
            <div style={style.menuArea}>
                {bl.map(this.block.bind(this))}
            </div>
        );
    }
}
