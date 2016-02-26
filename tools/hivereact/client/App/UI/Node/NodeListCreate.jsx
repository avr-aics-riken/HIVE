import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
import MenuNodeCreate from '../Menu/MenuNodeCreate.jsx';
import MenuNodeList from '../Menu/MenuNodeList.jsx';

export default class NodeListCreate extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            visible: false
        };

        this.visibility = false;
        this.store = this.props.store;
        this.action = this.props.action;
        this.styles = this.styles.bind(this);
        this.onClick = this.onClick.bind(this);
        this.onKeyDown = this.onKeyDown.bind(this);
        // this.props.menuStore.on(MenuStore.TOGGLE_MENU, this.toggleMenu.bind(this));

        this.componentDidMount = this.componentDidMount.bind(this);
        this.componentWillUnmount = this.componentWillUnmount.bind(this);
    }

    componentDidMount() {
        // window.addEventListener('mousemove', this.onMouseMove);
        // window.addEventListener('mouseup', this.onMouseUp);
        window.addEventListener('dblclick', this.onClick);
        window.addEventListener('keydown', this.onKeyDown);
        // this.props.store.on(Core.Constants.NODE_CHANGED, this.nodeChanged);
        // this.props.store.on(Core.Constants.NODE_SELECTE_CHANGED, this.selectChanged);
        // this.props.nodeStore.on(Store.NODE_MOVED, this.moveNode);
    }

    componentWillUnmount() {
        // window.removeEventListener('mousemove', this.onMouseMove);
        // window.removeEventListener('mouseup', this.onMouseUp);
        window.removeEventListener('dblclick', this.onClick);
        window.removeEventListener('keydown', this.onKeyDown);
        // this.props.store.removeListener(Core.Constants.NODE_CHANGED, this.nodeChanged);
        // this.props.store.removeListener(Core.Constants.NODE_SELECTE_CHANGED, this.selectChanged);
        // this.props.nodeStore.removeListener(Store.NODE_MOVED, this.moveNode);
    }

    onClick(eve){
        this.visibility = !this.visibility;
        this.setState({visible: this.visibility});
        if(this.visibility){
            setTimeout(()=>{
                this.refs.creator.refs.suggest.input.focus();
            }, 50);
        }
    }

    onKeyDown(eve){
        switch(eve.keyCode){
            case 27:
                this.visibility = false;
                break;
            case 32:
                this.visibility = true;
                setTimeout(()=>{
                    this.refs.creator.refs.suggest.input.focus();
                }, 50);
                break;
            default:
                break;
        }
        this.setState({visible: this.visibility});
    }

    styles() {
        return {
            menuArea: {
                backgroundColor: "rgba(16, 96, 8, 0.5)",
                color: "#eee",
                fontSize: "10pt",
                margin: "0px",
                padding: "0px",
                width: "200px",
                height: "200px",
                position: "absolute",
                bottom: "200px",
                left: "200px",
                zIndex: "9999",
                float: "left",
                display: this.state.visible ? "block" : "none"
            }
        };
    }

    render(){
        const style = this.styles();
        return (
            <div style={style.menuArea}>
                <MenuNodeCreate
                    store={this.props.store}
                    action={this.props.action}
                    ref="creator"
                />
                <MenuNodeList
                    store={this.props.store}
                    action={this.props.action}
                />
            </div>
        );
    }
}

