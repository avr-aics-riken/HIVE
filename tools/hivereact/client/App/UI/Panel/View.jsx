import React from "react";
import ReactDOM from "react-dom";
import Container from "./Container.jsx";
import Core from '../../Core';

export default class View extends React.Component {
    constructor(props) {
        super(props);

        this.store = props.store;
        this.action = props.action;
        this.state = {
            nodes: [].concat(this.store.getNodes())
        };

        this.styles = this.styles.bind(this);
        this.generator = this.generator.bind(this);
        this.nodeCountChanged = this.nodeCountChanged.bind(this);

        this.onMouseDown = this.onMouseDown.bind(this);
        this.componentDidMount = this.componentDidMount.bind(this);
        this.componentWillUnmount = this.componentWillUnmount.bind(this);
    }

    componentDidMount(){
        this.props.store.on(Core.Constants.NODE_COUNT_CHANGED, this.nodeCountChanged);
    }

    componentWillUnmount(){
        this.props.store.removeListener(Core.Constants.NODE_COUNT_CHANGED, this.nodeCountChanged);
    }

    nodeCountChanged(err, data){
        this.setState({nodes: [].concat(this.store.getNodes())});
    }

    onMouseDown(){
        this.props.action.unSelectNode([], this.props.nodeVarname);
    }

    styles() {
        return {
            container: {
                margin : "0px",
                padding : "0px",
                width : "100%",
                height: "100%",
                position: "absolute"
            }
        };
    }

    generator(node, key) {
        return (
            <Container
                store={this.store}
                action={this.action}
                node={node}
                key={node.varname + key}
            />
        );
    }

    render() {
        var styles = this.styles();
        var a = (
            <div style={styles.container} onMouseDown={this.onMouseDown}>
                {this.state.nodes.map((value, key)=>{
                    return this.generator(value, key);
                })}
            </div>
        );
        return a;
    }
}
