import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
import RcMenu, {SubMenu, Item as MenuItem, Divider} from 'rc-menu';
import velocity from 'velocity-animate';

// class
export default class MenuTop extends React.Component {
    constructor(props) {
        super(props);

        this.action = this.props.action;
        this.store = this.props.store;

        this.allClearNode = this.allClearNode.bind(this);
        this.handleClick = this.handleClick.bind(this);
        this.saveButton = this.saveButton.bind(this);
        this.loadButton = this.loadButton.bind(this);
        this.loadButtonClick = this.loadButtonClick.bind(this);
        
        this.showConsole = props.consoleShow;
    }

    allClearNode(disableConfirm){
        var flg = true;
        if(!disableConfirm){flg = confirm('really?');}
        if(flg){
            let nodes = this.props.store.getNodes();
            let plugs = this.props.store.getPlugs();
            for(let i = plugs.length - 1; i >= 0; --i){
                this.props.action.deletePlug(plugs[i]);
            }
            for(let i = nodes.length - 1; i >= 0; --i){
                this.props.action.deleteNode(nodes[i].varname);
            }
            return true;
        }else{
            return false;
        }
    }
    saveButton(eve){
        var data = {
            nodes: this.props.store.getNodes(""),
            plugs: this.props.store.getPlugs("")
        };
        var blob = new Blob([JSON.stringify(data, null, 2)], {type: "text/plain;charset=utf-8"});
        saveAs(blob, "save.json");
    }
    loadButtonClick(){
        var e = ReactDOM.findDOMNode(this.refs.inputFile);
        e.click();
    }
    loadButton(eve){
        if(eve.currentTarget.files && eve.currentTarget.files.length > 0){
            this.allClearNode.bind(this)(true);
            var reader = new FileReader();
            reader.onload = function(){
                let data = (JSON.parse(reader.result));
                if(data.nodes && data.nodes.length > 0){
                    for(let i in data.nodes){
                        this.props.action.importNode(data.nodes[i]);
                    }
                }else{
                    console.log('import failed: nodes.length === 0');
                }
                if(data.plugs && data.plugs.length > 0){
                    for(let i in data.plugs){
                        this.props.action.addPlug(data.plugs[i]);
                    }
                }else{
                    console.log('import failed: plugs.length === 0');
                }
            }.bind(this);
            reader.readAsText(eve.currentTarget.files[0]);
        }
    }
    exportButton(eve){
        console.log('"export function" called!');
    }
    
    // Edit menu
    editNodeCopy() { this.props.action.copy(); }
    editNodePaste() { this.props.action.paste(); }
    editNodeDelete() { this.props.action.delete(); }
    editNodeMakeGroup() { this.props.action.makeGroup(); }

    // Layout menu
    layoutAll() { this.props.action.setLayout('all'); }
    layoutNode() { this.props.action.setLayout('node'); }
    layoutPanel() { this.props.action.setLayout('panel'); }
    
    // Window menu
    windowToggleConsoleOutput() {
        this.showConsole = !this.showConsole;
        this.props.action.showConsoleOutput(this.showConsole);
    }
    
    // ここでメニュー操作時の動作を定義
    // info のなかの key を見て分岐したりする
    handleClick(info){
        let key = parseInt(info.key, 10);
        try {
            this[info.key](info.value);
        } catch (e) {
            console.error("Unknown menu command", info, e);
        }
    }
 
    // render
    render(){
        // ここでメニューの構造定義
        const horizontalMenu = React.cloneElement((
            <RcMenu onClick={this.handleClick}>
                <SubMenu title={<span>File</span>} key="file">
                    <MenuItem key="loadButtonClick">Load</MenuItem>
                    <MenuItem key="saveButton">Save</MenuItem>
                    <MenuItem key="exportButton">Export</MenuItem>
                    <MenuItem key="allClearNode">Clear all</MenuItem>
                </SubMenu>
                <SubMenu title={<span>Edit</span>} key="edit">
                    <MenuItem key="editNodeCopy">Copy</MenuItem>
                    <MenuItem key="editNodePaste">Paste</MenuItem>
                    <MenuItem key="editNodeDelete">Delete</MenuItem>
                    <MenuItem key="editNodeMakeGroup">MakeGroup</MenuItem>
                </SubMenu>
                <SubMenu title={<span>Layout</span>} key="layout">
                    <MenuItem key="layoutAll">All</MenuItem>
                    <MenuItem key="layoutNode">Node mode</MenuItem>
                    <MenuItem key="layoutPanel">Panel mode</MenuItem>
                </SubMenu>
                <SubMenu title={<span>Window</span>} key="Window">
                    <MenuItem key="windowToggleConsoleOutput">Console Output</MenuItem>
                </SubMenu>
                </RcMenu>
        ), {
            mode: 'horizontal',
            openAnimation: 'slide-up',
            openSubMenuOnMouseEnter: false,
            closeSubMenuOnMouseLeave: true
        });

        let style = {
            width: "100%",
            // position: "absolute",
            // top: "0px",
            // left: "0px",
            zIndex: "99999"
        };
        return (
            <div style={{position: "relative", width: "100%", zIndex: "99999"}}>
                <div style={style}>{horizontalMenu}</div>
                <input type="file" ref="inputFile" style={{display: "none"}} onChange={this.loadButton} />
            </div>
        );
    }
}

// ユーティリティ類の定義
const animation = {
    enter(node, done) {
        let ok = false;
        function complete() {
            if (!ok) {
                ok = 1;
                done();
            }
        }
        node.style.display = 'none';
        velocity(node, 'slideDown', {
            duration: 300,
            complete: complete,
        });
        return {
            stop() {
                velocity(node, 'finish');
                // velocity complete is async
                complete();
            },
        };
    },

    appear() {
        return this.enter.apply(this, arguments);
    },

    leave(node, done) {
        let ok = false;
        function complete() {
            if (!ok) {
                ok = 1;
                done();
            }
        }
        node.style.display = 'block';
        velocity(node, 'slideUp', {
            duration: 300,
            complete: complete,
        });
        return {
            stop() {
                velocity(node, 'finish');
                // velocity complete is async
                complete();
            },
        };
    },
};
