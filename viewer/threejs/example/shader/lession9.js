import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import Stats from "three/addons/libs/stats.module.js";
// import { GUI } from "three/addons/libs/lil-gui.module.min.js";
import GUI from "https://cdn.jsdelivr.net/npm/lil-gui@0.19/+esm";

// https://juejin.cn/post/7386299931398914098

//  链接：https://learnopengl.com/Getting-started/Coordinate-Systems

/**
 * 对于 mesh 的旋转、平移、缩放其实会以 modelMatrix 模型矩阵的形式作用到顶点上，
 * 从而使几何体的顶点从本地/模型坐标空间(local space/model space/local coordinates)变成当前场景的世界坐标空间(world space/world coordinates)，
 * 每个物体由基于自身的坐标(position)变成3D场景里坐标(modelPosition)
 */
const renderer = new THREE.WebGLRenderer({
    antialias: true, // 抗锯齿
});
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

const camera = new THREE.PerspectiveCamera(
    75,1,
    0.01, 100
);
camera.position.set(0, 0, 5.4);

const scene = new THREE.Scene();
scene.background = new THREE.Color(0x8899ff);

const controls = new OrbitControls(camera, renderer.domElement);
const stats = new Stats();
const gui = new GUI();
const clock = new THREE.Clock();


const one = new THREE.Mesh();
function example1() {
    const vertexShader = /* GLSL */ `
    uniform float uTime;
    varying vec2 vUv;
    varying float vStrength;
    const float PI = 3.1415926;

    void main() {
        vUv = uv;

        vec4 modelPosition = modelMatrix * vec4(position, 1.0);

        float dist = distance(uv, vec2(0.5));
        float strength = smoothstep(0.4, 0.0, dist);;
        vStrength = strength;
        modelPosition.y += 0.3 * sin(uTime) * strength;
        gl_Position = projectionMatrix * viewMatrix * modelPosition;
    }
    `;

    const fragmentShader = /* GLSL */ `
    uniform sampler2D uTexture;
    varying vec2 vUv;
    varying float vStrength;

    void main() {
        gl_FragColor = texture2D(uTexture, vUv);
    }
    `;

    one.geometry = new THREE.PlaneGeometry(1, 1, 30, 30);

    one.material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
            uStrength: { value: 0.5 },
            uFrequency: { value: 5.0 },
            uTexture: {
                value: new THREE.TextureLoader().load("./spain-flag.webp"),
            },
        },
        vertexShader: vertexShader,
        fragmentShader: fragmentShader,
        // wireframe: true,
    });

    // three.rotation.x = -Math.PI / 2;
    one.position.set(-1.0, 0, 0);
    scene.add(one);
}

example1();


const three = new THREE.Mesh();
function example3() {
    const vertexShader = /* GLSL */ `
    uniform float uTime;
    varying vec2 vUv;
    varying float vStrength;
    const float PI = 3.1415926;

    void main() {
        vUv = uv;

        vec4 modelPosition = modelMatrix * vec4(position, 1.0);
        // modelPosition.y += 0.08 * sin(modelPosition.x * PI * 2.0 + uTime);
        // modelPosition.y += 0.1 * sin(modelPosition.z * PI * 1.5 + uTime);

        float strength = smoothstep(0.3, 0.0, abs(uv.x - 0.5 + 0.5 * sin(uTime)));
        vStrength = strength;
        modelPosition.y += 0.3 * strength;
        gl_Position = projectionMatrix * viewMatrix * modelPosition;
    }
    `;

    const fragmentShader = /* GLSL */ `
    uniform sampler2D uTexture;
    varying vec2 vUv;
    varying float vStrength;

    void main() {
        gl_FragColor = texture2D(uTexture, vUv);
    }
    `;

    three.geometry = new THREE.PlaneGeometry(1, 1, 30, 30);

    three.material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
            uStrength: { value: 0.5 },
            uFrequency: { value: 5.0 },
            uTexture: {
                value: new THREE.TextureLoader().load("./spain-flag.webp"),
            },
        },
        vertexShader: vertexShader,
        fragmentShader: fragmentShader,
        // wireframe: true,
    });

    // three.rotation.x = -Math.PI / 2;
    three.position.set(1.0, 0, 0);
    scene.add(three);
}
example3();

gui.add(three.material.uniforms.uStrength, "value", 0, 1, 0.01).name(
    "uStrength"
);

gui.add(three.material.uniforms.uFrequency, "value", 0, 20, 0.01).name(
    "uFrequency"
);

let time = 0;
function animate() {
    time += 0.05;
    one.material.uniforms.uTime.value = time;
    // two.material.uniforms.uTime.value = time;
    const delta = clock.getElapsedTime();
    three.material.uniforms.uTime.value = delta;
    // three.rotation.y = time;

    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}
animate();
