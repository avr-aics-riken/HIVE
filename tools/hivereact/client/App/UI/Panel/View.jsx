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

        this.props.store.on(Core.Constants.NODE_COUNT_CHANGED, function(err, data){
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
                height: "100%"
            },
            menu: {
                backgroundColor: "black",
                border: "1px solid #999",
                color: "#eee",
                minWidth: "100px",
                height: "20px",
                position: "absolute",
                left: "0px",
                top: "0px"
            },
            button: {
                backgroundColor: "#666",
                height: "20px",
                padding: "0px 5px",
                float: "left",
                boxShadow: "0px 0px 1px 0px white inset"
            }
        };
    }

    menuButton(value, key){
        var styles = this.styles();
        return (
            <div style={styles.button} key={key}>{value}</div>
        );
    }

    menuArea(){
        var styles = this.styles();
        return (
            <div style={styles.menu}>
                {['button1', 'button2', 'button3'].map(this.menuButton.bind(this))}
            </div>
        );
    }

    generator(node, key) {
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
                {this.menuArea.bind(this)()}
                {this.state.nodes.map((value, key)=>{
                    return this.generator(value, key);
                })}
            </div>
        );
        return a;
    }
}
