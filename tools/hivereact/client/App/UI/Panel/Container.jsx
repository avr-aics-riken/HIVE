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
                <div style={styles.panelTitleBar}>
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
