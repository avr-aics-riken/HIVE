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
            nodes: this.props.store.getNodes(),
            plugs: this.props.store.getPlugs()
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

    // ここでメニュー操作時の動作を定義
    // info のなかの key を見て分岐したりする
    handleClick(info){
        let key = parseInt(info.key, 10);
        switch(key){
            case 1:
                this.loadButtonClick();
                break;
            case 2:
                this.saveButton();
                break;
            case 3:
                console.log('"export function" called!');
                break;
            case 4:
                this.allClearNode();
                break;
            case 11:
				this.props.action.copy();
                break;
            case 12:
				this.props.action.paste();
                break;
            case 13:
				this.props.action.delete();
                break;
        }
        console.log(info);
    }

    // render
    render(){
        // ここでメニューの構造定義
        const horizontalMenu = React.cloneElement((
            <RcMenu onClick={this.handleClick}>
                <SubMenu title={<span>File</span>} key="0">
                    <MenuItem key="1">Load</MenuItem>
                    <MenuItem key="2">Save</MenuItem>
                    <MenuItem key="3">Export</MenuItem>
                    <MenuItem key="4">Clear all</MenuItem>
                </SubMenu>
                <SubMenu title={<span>Edit</span>} key="10">
                    <MenuItem key="11">Copy</MenuItem>
                    <MenuItem key="12">Paste</MenuItem>
                    <MenuItem key="13">Delete</MenuItem>
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
