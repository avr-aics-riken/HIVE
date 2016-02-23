import React from "react"
import ReactDOM from "react-dom"
import Container from "./Container.jsx"
import Core from '../../Core'

export default class View extends React.Component {
    constructor(props) {
        super(props);

        this.store = props.store;
        this.action = props.action;
        this.state = {
            nodes: [].concat(this.store.getNodes()),
            position: {x: 0, y: 0}
        };

        this.styles = this.styles.bind(this);
        this.generator = this.generator.bind(this);

        this.props.store.on(Core.Store.NODE_CHANGED, function(err, data){
            this.setState({nodes: [].concat(this.store.getNodes())});
        }.bind(this));
        this.props.store.on(Core.Store.NODE_COUNT_CHANGED, function(err, data){
            this.setState({nodes: [].concat(this.store.getNodes())});
        }.bind(this));
    }

    styles() {
        return {
            container: {
                backgroundColor: "#333",
                margin : "0px",
                padding : "0px",
                width : "100%",
                height: "100%",
                overflow: "hidden"
            }
        }
    }

    generator(node, key) {
        var styles = this.styles();
        return (
            <Container
                store={this.store}
                action={this.action}
                node={node}
                key={key}
            />
        );
    }

    render() {
        var styles = this.styles();
        var a = (
            <div style={styles.container}>
                {this.state.nodes.map((value, key)=>{
                    return this.generator(value, key);
                })}
            </div>
        );
        return a;
    }
}
