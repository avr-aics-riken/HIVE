import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'

export default class Container extends React.Component {
    constructor(props) {
        super(props);

        this.store = props.store;
        this.action = props.action;
        this.state = {
            node: null,
            position: {x: 0, y: 0}
        };
        this.node = props.node;
        let nodes = this.props.store.getNodes();
        for(let i = 0; i < nodes.length; ++i){
            if(nodes[i].varname === this.props.node.varname){
                this.state.node = nodes[i];
                break;
            }
        }

        this.componentDidMount = this.componentDidMount.bind(this);
        this.componentWillUnmount = this.componentWillUnmount.bind(this);
        this.onMouseMove = this.onMouseMove.bind(this);
        this.onMouseUp = this.onMouseUp.bind(this);
        this.onMouseDown = this.onMouseDown.bind(this);
    }

    componentDidMount() {
        window.addEventListener('mousemove', this.onMouseMove);
        window.addEventListener('mouseup', this.onMouseUp);
        this.props.store.on(Core.Store.COMPONENT_CHANGED, this.componentChanged);
    }

    componentWillUnmount() {
        window.removeEventListener('mousemove', this.onMouseMove);
        window.removeEventListener('mouseup', this.onMouseUp);
        this.props.store.removeListener(Core.Store.COMPONENT_CHANGED, this.componentChanged);
    }

    onMouseDown(ev) {
        if(ev.button === 0){
            this.isLeftDown = true;
            this.mousePos = {x: ev.clientX, y: ev.clientY};
            this.offsetLeft = ev.currentTarget.offsetLeft;
            this.offsetTop = ev.currentTarget.offsetTop;
        }
    }

    onMouseUp(ev) {
        this.isLeftDown = false;
    }

    onMouseMove(ev) {
        if(this.isLeftDown){
            let mv = {x : ev.clientX - this.mousePos.x, y: ev.clientY - this.mousePos.y};
            this.setState({position: {x: this.offsetLeft + mv.x, y: this.offsetTop + mv.y}});
        }
    }

    // /// 閉じるボタンが押された.
    // onCloseClick(ev) {
    //     this.props.action.deleteNode(this.props.node.varname);
    // }
    //
    // /// 閉じるボタンにマウスホバーされた
    // onCloseHover(ev) {
    //     this.setState({ closeHover : !this.state.closeHover })
    // }
    //
    styles() {
        return {
            container : {
                backgroundColor: "#666",
                margin : "0px",
                padding : "0px",
                minWidth : "100px",
                minHeight: "100px",
                position: "absolute",
                top: "10px",
                right: "10px",
                boxShadow : "0px 0px 3px 0px skyblue inset"
            },
            panelTitleBar: {
                backgroundColor: "silver",
                color: "#333",
                fontSize: "8pt",
                lineHeight: "24px",
                minHeight: "24px",
                margin : "0px",
                padding : "0px 5px"
            },
            panelCloseButton: {
                backgroundColor : "#ea4412",
                borderRadius : "5px",
                color: "white",
                fontWeight: "bold",
                lineHeight: "15px",
                textAlign : "center",
                margin : "4px",
                width: "15px",
                height: "15px",
                position : "absolute",
                top : "0px",
                right : "0px"
            },
            panelScale: {
                backgroundColor: "orange",
                position: "absolute",
                bottom: "0px",
                right: "0px",
                width: "15px",
                height: "15px",
                overflow: "hidden"
            }
        }
    }

    render() {
        var styles = this.styles();
        return (
            <div style={styles.container}>
                <div style={styles.panelTitleBar} onMouseDown={this.onMouseDown.bind(this)}>
                    {this.props.title}
                    <div style={styles.panelCloseButton}>x</div>
                </div>
                {this.props.component}
                <div style={styles.panelScale}>
                </div>
            </div>
        );
    }
}
