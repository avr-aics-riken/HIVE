import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'
import MenuStore from './MenuStore.jsx'
import MenuNodeCreate from './MenuNodeCreate.jsx'
import MenuNodeList from './MenuNodeList.jsx'

export default class Menu extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes: null,
            visible: this.props.menuStore.getVisible()
        };

        this.store = this.props.store;
        this.action = this.props.action;
        this.menuStore = this.props.menuStore;
        this.menuAction = this.props.menuAction;
        this.styles = this.styles.bind(this);
        this.onClick = this.onClick.bind(this);
        this.props.menuStore.on(MenuStore.TOGGLE_MENU, this.toggleMenu.bind(this));
    }

    // functionReplacer(data){
    //     return JSON.stringify(data, function(key, val){
    //         if(typeof val === "function"){
    //             return val.toString();
    //         }
    //         return val;
    //     }, 2);
    // }
    //
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

    onClick(eve){
        let f = this.menuStore.getVisible();
        this.props.menuAction.toggleMenu(!f);
    }

    toggleMenu(){
        let f = this.menuStore.getVisible();
        this.setState({visible: f});
    }

    styles() {
        return {
            menuArea: {
                backgroundColor: "maroon",
                color: "#eee",
                fontSize: "10pt",
                margin: "0px",
                padding: "0px",
                width: "300px",
                height: "100%",
                position: "fixed",
                top: "0px",
                left: this.state.visible ? "0px" : "-300px",
                zIndex: "9999",
                transition: "left 0.5s cubic-bezier(0.18, 0.93, 0.26, 0.99)",
                overflow: "auto" // temp
            },
            header: {
                backgroundColor: "#200",
                textAlign: "30px",
                margin: "0px",
                padding: "0px",
                width: "100%",
                minHeight: "30px",
                boxShadow: "0px 0px 1px 0px white inset"
            },
            headerCaption: {
                color: "lightpink",
                fontWeight: "bold",
                padding: "5px 10px",
                textShadow: "0px 0px 2px deeppink"
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
                margin: "-1px 0px 0px",
                padding: "0px 12px 0px 5px",
                height: "15px",
                display: "inline-block",
                float: "left",
                boxShadow: "0px -1px 1px 1px #666 inset"
            },
            tagtip: {
                backgroundColor: "red",
                color: "#400",
                fontSize: "10pt",
                fontWeight: "bold",
                margin: "0px",
                padding: "0px 5px",
                width: "50px",
                height: "20px",
                position: "fixed",
                top: "15px",
                left: this.state.visible ? "-15px" : "-315px",
                transform: "rotate(90deg) translateY(-300px)",
                transition: "left 0.5s cubic-bezier(0.18, 0.93, 0.26, 0.99)",
                cursor: "pointer"
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
                    // var blob = new Blob([this.functionReplacer.bind(this)(data)], {type: "text/plain;charset=utf-8"});
                    var blob = new Blob([data], {type: "text/plain;charset=utf-8"});
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
                <div>
                    <div style={style.header}>
                        <div style={style.headerCaption}>Node Create</div>
                    </div>
                    <MenuNodeCreate
                        store={this.props.store}
                        action={this.props.action}
                        menuStore={this.props.menuStore}
                        menuAction={this.props.menuAction}
                    />
                    <MenuNodeList
                        store={this.props.store}
                        action={this.props.action}
                        menuStore={this.props.menuStore}
                        menuAction={this.props.menuAction}
                    />
                </div>
                <div>
                    <div style={style.header}>
                        <div style={style.headerCaption}>Others</div>
                    </div>
                    {bl.map(this.block.bind(this))}
                </div>
                <div style={style.tagtip} onClick={this.onClick.bind(this)}>menu</div>
            </div>
        );
    }
}

                // <div style={style.tagtip}>menu</div>

