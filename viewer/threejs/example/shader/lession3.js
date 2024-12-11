import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import Stats from "three/addons/libs/stats.module.js";

// https://juejin.cn/post/7259411780375314490
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

const camera = new THREE.PerspectiveCamera(
    45,
    window.innerWidth / window.innerHeight,
    1,
    500
);
camera.position.set(0, 0, 5);

const scene = new THREE.Scene();
scene.background = new THREE.Color(0x8899ff);

const controls = new OrbitControls(camera, renderer.domElement);
const stats = new Stats();

const one = new THREE.Mesh();
function example1() {
    const vertex = /* GLSL */ `
    varying vec2 vUv;

    void main() {
      vUv = uv;

      // gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
      gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
    }
  `;

    const fragment = /* GLSL */ `
  uniform float uTime;
  varying vec2 vUv;

  void main() {
    // 绘制渐变圆形
    float dist = length(fract(vUv * 5.0) - vec2(0.5));
    float radius = 0.5 * (sin(uTime + vUv.x + vUv.y) *0.5 + 0.5); // sin -1 ~ 1, *0.5 --> -0.5 ~ 0.5, +0.5 --> 0 ~ 1
    vec3 color = vec3(step(radius, dist));
    gl_FragColor = vec4(color, 1.0);
  }
  `;

    one.geometry = new THREE.BoxGeometry(1, 1, 1);

    one.material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
        },
        vertexShader: vertex,
        fragmentShader: fragment,
    });

    one.position.set(-1, 0, 0);
    scene.add(one);
}

example1();

const two = new THREE.Mesh();
function example2() {
    const vertex = /* GLSL */ `
    varying vec2 vUv;

    void main() {
        vUv = uv;

        // gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
        gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
    }
    `;

    const fragment = /* GLSL */ `
    varying vec2 vUv;
    uniform float uTime;

    void main() {
        // 先居中，再绘制圆形
        float dist = fract((length(vUv - vec2(0.5)) / 0.707 - uTime *  0.1 ) * 5.0);
        float radius = 0.5;
        vec3 color = vec3(step(radius, dist));
        gl_FragColor = vec4(color, 1.0);
    }
    `;

    two.geometry = new THREE.PlaneGeometry(1, 1);

    two.material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
        },
        vertexShader: vertex,
        fragmentShader: fragment,
        // wireframe: true,
    });

    two.position.set(1, 0, 0);
    scene.add(two);
}
example2();

let time = 0;
function animate() {
    time += 0.05;
    one.material.uniforms.uTime.value = time;
    two.material.uniforms.uTime.value = time;
    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}
animate();
