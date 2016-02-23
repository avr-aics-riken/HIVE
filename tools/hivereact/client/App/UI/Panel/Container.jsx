import React from "react"
import ReactDOM from "react-dom"

export default class Container extends React.Component {
    constructor(props) {
        super(props);

        this.store = props.store;
        this.action = props.action;

        // this.componentDidUpdate = this.componentDidUpdate.bind(this);
        // this.componentDidMount = this.componentDidMount.bind(this);
    }

    // componentDidUpdate() {
    // }
    //
    // componentDidMount() {
    // }
    //
    // componentWillUnmount() {
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
                display: "inline-block",
                boxShadow : "0px 0px 3px 0px skyblue inset"
            },
            panelTitleBar: {
                backgroundColor: "silver",
                color: "#333",
                fontSize: "8pt",
                margin : "0px",
                padding : "2px 5px",
                minHeight: "10px"
            },
            panelCloseButton: {
                backgroundColor: "red",
                color: "white"
            },
            panelScale: {
                backgroundColor: "green"
            }
        }
    }

    render() {
        var styles = this.styles();
        return (
            <div style={styles.container}>
                <div style={styles.panelTitleBar}>
                    <p>{this.props.title}</p>
                    <div style={styles.panelCloseButton}>x</div>
                </div>
                {this.props.component}
                <div style={styles.panelScale}>
                </div>
            </div>
        );
    }
}
